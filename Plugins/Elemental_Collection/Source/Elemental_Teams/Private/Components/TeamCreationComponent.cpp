// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Components/TeamCreationComponent.h"
#include "Teams/TeamPublicInfo.h"
#include "Teams/TeamPrivateInfo.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/ExperienceManagerInterface.h"
#include "Interfaces/TeamAgentInterface.h"
#include "Interfaces/GameModeInterface.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(TeamCreationComponent)

UTeamCreationComponent::UTeamCreationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PublicTeamInfoClass = ATeamPublicInfo::StaticClass();
	PrivateTeamInfoClass = ATeamPrivateInfo::StaticClass();
}

#if WITH_EDITOR
EDataValidationResult UTeamCreationComponent::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	//@TODO: TEAMS: Validate that all display assets have the same properties set!

	return Result;
}
#endif

void UTeamCreationComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for the experience load to complete via interface
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	UActorComponent* ExperienceComponent = GameState->FindComponentByInterface(UExperienceManagerInterface::StaticClass());
	if (IExperienceManagerInterface* ExperienceInterface = Cast<IExperienceManagerInterface>(ExperienceComponent))
	{
		ExperienceInterface->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void UTeamCreationComponent::OnExperienceLoaded(const UExperienceDefinition* Experience)
{
#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		ServerCreateTeams();
		ServerAssignPlayersToTeams();
	}
#endif
}

#if WITH_SERVER_CODE

void UTeamCreationComponent::ServerCreateTeams()
{
	for (const auto& KVP : TeamsToCreate)
	{
		const int32 TeamId = KVP.Key;
		ServerCreateTeam(TeamId, KVP.Value);
	}
}

void UTeamCreationComponent::ServerAssignPlayersToTeams()
{
	// Assign players that already exist to teams
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (Cast<ITeamAgentInterface>(PS) != nullptr)
		{
			ServerChooseTeamForPlayer(PS);
		}
	}

	// Listen for new players logging in via IGameModeInterface
	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AGameModeBase>())
	{
		if (IGameModeInterface* GameModeInterface = Cast<IGameModeInterface>(GameMode))
		{
			if (FOnGameModePlayerInitialized* Delegate = GameModeInterface->GetOnPlayerInitializedDelegate())
			{
				Delegate->AddUObject(this, &ThisClass::OnPlayerInitialized);
			}
		}
	}
}

void UTeamCreationComponent::ServerChooseTeamForPlayer(APlayerState* PS)
{
	check(PS);
	if (ITeamAgentInterface* TeamAgent = Cast<ITeamAgentInterface>(PS))
	{
		if (PS->IsOnlyASpectator())
		{
			TeamAgent->SetGenericTeamId(FGenericTeamId::NoTeam);
		}
		else
		{
			TeamAgent->SetGenericTeamId(IntegerToGenericTeamId(GetLeastPopulatedTeamID()));
		}
	}
}

void UTeamCreationComponent::OnPlayerInitialized(AGameModeBase* GameMode, AController* NewPlayer)
{
	check(NewPlayer);
	check(NewPlayer->PlayerState);
	if (Cast<ITeamAgentInterface>(NewPlayer->PlayerState) != nullptr)
	{
		ServerChooseTeamForPlayer(NewPlayer->PlayerState);
	}
}

void UTeamCreationComponent::ServerCreateTeam(int32 TeamId, UTeamDisplayAsset* DisplayAsset)
{
	check(HasAuthority());

	//@TODO: ensure the team doesn't already exist

	UWorld* World = GetWorld();
	check(World);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ATeamPublicInfo* NewTeamPublicInfo = World->SpawnActor<ATeamPublicInfo>(PublicTeamInfoClass, SpawnInfo);
	checkf(NewTeamPublicInfo != nullptr, TEXT("Failed to create public team actor from class %s"), *GetPathNameSafe(*PublicTeamInfoClass));
	NewTeamPublicInfo->SetTeamId(TeamId);
	NewTeamPublicInfo->SetTeamDisplayAsset(DisplayAsset);

	ATeamPrivateInfo* NewTeamPrivateInfo = World->SpawnActor<ATeamPrivateInfo>(PrivateTeamInfoClass, SpawnInfo);
	checkf(NewTeamPrivateInfo != nullptr, TEXT("Failed to create private team actor from class %s"), *GetPathNameSafe(*PrivateTeamInfoClass));
	NewTeamPrivateInfo->SetTeamId(TeamId);
}

int32 UTeamCreationComponent::GetLeastPopulatedTeamID() const
{
	const int32 NumTeams = TeamsToCreate.Num();
	if (NumTeams > 0)
	{
		TMap<int32, uint32> TeamMemberCounts;
		TeamMemberCounts.Reserve(NumTeams);

		for (const auto& KVP : TeamsToCreate)
		{
			const int32 TeamId = KVP.Key;
			TeamMemberCounts.Add(TeamId, 0);
		}

		AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
		for (APlayerState* PS : GameState->PlayerArray)
		{
			if (const ITeamAgentInterface* TeamAgent = Cast<ITeamAgentInterface>(PS))
			{
				const int32 PlayerTeamID = GenericTeamIdToInteger(TeamAgent->GetGenericTeamId());
				if ((PlayerTeamID != INDEX_NONE) && !PS->IsInactive())	// do not count unassigned or disconnected players
				{
					check(TeamMemberCounts.Contains(PlayerTeamID))
					TeamMemberCounts[PlayerTeamID] += 1;
				}
			}
		}

		// sort by lowest team population, then by team ID
		int32 BestTeamId = INDEX_NONE;
		uint32 BestPlayerCount = TNumericLimits<uint32>::Max();
		for (const auto& KVP : TeamMemberCounts)
		{
			const int32 TestTeamId = KVP.Key;
			const uint32 TestTeamPlayerCount = KVP.Value;

			if (TestTeamPlayerCount < BestPlayerCount)
			{
				BestTeamId = TestTeamId;
				BestPlayerCount = TestTeamPlayerCount;
			}
			else if (TestTeamPlayerCount == BestPlayerCount)
			{
				if ((TestTeamId < BestTeamId) || (BestTeamId == INDEX_NONE))
				{
					BestTeamId = TestTeamId;
					BestPlayerCount = TestTeamPlayerCount;
				}
			}
		}

		return BestTeamId;
	}

	return INDEX_NONE;
}
#endif	// WITH_SERVER_CODE
