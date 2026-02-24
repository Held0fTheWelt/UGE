// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "States/PlayerState_Elemental.h"
#include "GameFeatureEvents.h"

#include "AbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Engine/World.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Structs/VerbMessageTypes.h"
#include "Net/UnrealNetwork.h"
#include "Interfaces/TeamAgentInterface.h"

#include "Interfaces/ExperienceManagerInterface.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/GameModeInterface.h"

#include "Interfaces/AbilitySystemComponentInterface.h"
#include "Interfaces/ExperienceManagerInterface.h"
#include "Interfaces/PawnExtensionComponentInterface.h"
#include "Interfaces/PawnDataAbilitySetProviderInterface.h"
#include "Subsystems/SquadSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PlayerState_Elemental)

class AController;
class APlayerState;
class FLifetimeProperty;

APlayerState_Elemental::APlayerState_Elemental(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MyPlayerConnectionType(EPlayerConnectionType::Player)
{
	// AbilitySystemComponent is set by a component (e.g. PawnExtension); not created here so the hierarchy stays in Core.
	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);

	MyTeamID = FGenericTeamId::NoTeam;
	MySquadID = INDEX_NONE;
}

void APlayerState_Elemental::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void APlayerState_Elemental::Reset()
{
	Super::Reset();
	
	ResetAbilitySystem();
}

void APlayerState_Elemental::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);

	if (APawn* Pawn = GetPawn())
	{
		UActorComponent* PawnExtComp = Pawn->FindComponentByInterface(UPawnExtensionComponentInterface::StaticClass());
		if (IPawnExtensionComponentInterface* ExtInterface = Cast<IPawnExtensionComponentInterface>(PawnExtComp))
		{
			ExtInterface->CheckDefaultInitialization();
		}
	}
}

void APlayerState_Elemental::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	// Stat tags can be copied when the other state implements the same stat interface (e.g. APlayerState_Elemental)
	if (APlayerState_Elemental* OtherElementalPS = Cast<APlayerState_Elemental>(PlayerState))
	{
		// Optional: copy StatTags / squad/team if desired; currently handled by replication
	}
}

void APlayerState_Elemental::OnDeactivated()
{
	SetPlayerConnectionType(EPlayerConnectionType::InactivePlayer);

	bool bDestroyDeactivatedPlayerState = true;
	if (UWorld* World = GetWorld())
	{
		if (AGameStateBase* GameState = World->GetGameState())
		{
			UActorComponent* ExperienceComponent = GameState->FindComponentByInterface(UExperienceManagerInterface::StaticClass());
			if (IExperienceManagerInterface* ExperienceInterface = Cast<IExperienceManagerInterface>(ExperienceComponent))
			{
				bDestroyDeactivatedPlayerState = ExperienceInterface->ShouldDestroyDeactivatedPlayerState();
			}
		}
	}

	if (bDestroyDeactivatedPlayerState)
	{
		Destroy();
	}
}

void APlayerState_Elemental::OnReactivated()
{
	if (GetPlayerConnectionType() == EPlayerConnectionType::InactivePlayer)
	{
		SetPlayerConnectionType(EPlayerConnectionType::Player);
	}
}

void APlayerState_Elemental::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyPlayerConnectionType, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyTeamID, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MySquadID, SharedParams);

	SharedParams.Condition = ELifetimeCondition::COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ReplicatedViewRotation, SharedParams);

	DOREPLIFETIME(ThisClass, StatTags);	
}

FRotator APlayerState_Elemental::GetReplicatedViewRotation() const
{
	// Could replace this with custom replication
	return ReplicatedViewRotation;
}

void APlayerState_Elemental::SetReplicatedViewRotation(const FRotator& NewRotation)
{
	if (NewRotation != ReplicatedViewRotation)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, ReplicatedViewRotation, this);
		ReplicatedViewRotation = NewRotation;
	}
}

APlayerController* APlayerState_Elemental::GetUnifiedPlayerController() const
{
	return Cast<APlayerController>(GetOwner());
}

UAbilitySystemComponent* APlayerState_Elemental::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
}

void APlayerState_Elemental::InitAbilityActorInfo(UObject* InOwnerActor, UObject* InAvatarActor)
{
	// Ability system is owned by a component; init is done in InitializeAbilitySystem().
}

void APlayerState_Elemental::ClearActorInfo()
{
	// Ability system is owned by a component; clear is done in ResetAbilitySystem().
}



void APlayerState_Elemental::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitializeAbilitySystem();

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		UActorComponent* ExperienceComponent = GameState->FindComponentByInterface(UExperienceManagerInterface::StaticClass());
		check(ExperienceComponent);
		if (IExperienceManagerInterface* ExperienceInterface = Cast<IExperienceManagerInterface>(ExperienceComponent))
		{
			ExperienceInterface->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
		}
	}
}

void APlayerState_Elemental::SetPawnData(const UObject* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	// Grant ability sets from PawnData via interface so Elemental does not depend on Core types.
	if (IPawnDataAbilitySetProviderInterface* Provider = Cast<IPawnDataAbilitySetProviderInterface>(const_cast<UObject*>(static_cast<const UObject*>(PawnData))))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		{
			Provider->GrantAbilitySetsToAbilitySystem(ASC, this);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, GameFeatureEvents::NAME_AbilityReady);
	
	ForceNetUpdate();
}

void APlayerState_Elemental::OnRep_PawnData()
{
	// PawnData has been replicated, components can now use it
}

void APlayerState_Elemental::SetPlayerConnectionType(EPlayerConnectionType NewType)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyPlayerConnectionType, this);
	MyPlayerConnectionType = NewType;
}

void APlayerState_Elemental::SetSquadID(int32 NewSquadId)
{
	if (HasAuthority())
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MySquadID, this);

		MySquadID = NewSquadId;
	}
}

void APlayerState_Elemental::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (HasAuthority())
	{
		const FGenericTeamId OldTeamID = MyTeamID;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamID, this);
		MyTeamID = NewTeamID;
		ITeamAgentInterface::ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot set team for %s on non-authority"), *GetPathName(this));
	}
}

FGenericTeamId APlayerState_Elemental::GetGenericTeamId() const
{
	return MyTeamID;
}

FOnTeamIndexChangedDelegate* APlayerState_Elemental::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void APlayerState_Elemental::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ITeamAgentInterface::ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void APlayerState_Elemental::OnRep_MySquadID()
{
	if (UWorld* World = GetWorld())
	{
		if (USquadSubsystem* SquadSubsystem = World->GetSubsystem<USquadSubsystem>())
		{
			SquadSubsystem->OnPlayerSquadIdChanged(this);
		}
	}
}

void APlayerState_Elemental::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void APlayerState_Elemental::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 APlayerState_Elemental::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool APlayerState_Elemental::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void APlayerState_Elemental::ClientBroadcastMessage_Implementation(const FVerbMessage Message)
{
	// This check is needed to prevent running the action when in standalone mode
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

void APlayerState_Elemental::InitializeAbilitySystem()
{
	check(AbilitySystemComponent);
	if (UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->InitAbilityActorInfo(this, GetPawn());
	}
}

void APlayerState_Elemental::ResetAbilitySystem()
{
	check(AbilitySystemComponent);
	if (UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->ClearActorInfo();
	}
}


void APlayerState_Elemental::OnExperienceLoaded(const UExperienceDefinition* CurrentExperience)
{
	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AGameModeBase>())
	{
		if (IGameModeInterface* GameModeInterface = Cast<IGameModeInterface>(GameMode))
		{
			if (const UObject* NewPawnData = GameModeInterface->GetPawnDataForController(GetOwningController()))
			{
				SetPawnData(NewPawnData);
			}
		}
	}
}
