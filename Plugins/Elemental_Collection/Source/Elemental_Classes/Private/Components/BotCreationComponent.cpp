// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Components/BotCreationComponent.h"
#include "Interfaces/BotSpawnProviderInterface.h"
#include "Interfaces/ExperienceManagerInterface.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/GameModeInterface.h"
//#include "GameFeatures/GameFeature_PawnExtension/Source/GameFeature_PawnExtensionRuntime/Public/Components/WarCollectionPawnExtensionComponent.h"
//#include "GameFeatures/GameFeature_HealthSystem/Source/GameFeature_HealthSystemRuntime/Public/Components/WarCollectionHealthComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BotCreationComponent)

UBotCreationComponent::UBotCreationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#include "GameFramework/GameStateBase.h"

void UBotCreationComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for the experience load to complete
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	UActorComponent* ExperienceComponent = GameState->FindComponentByInterface(UExperienceManagerInterface::StaticClass());
	check(ExperienceComponent);
	if (IExperienceManagerInterface* ExperienceInterface = Cast<IExperienceManagerInterface>(ExperienceComponent))
	{
		ExperienceInterface->CallOrRegister_OnExperienceLoaded_LowPriority(FOnExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void UBotCreationComponent::OnExperienceLoaded(const UExperienceDefinition* Experience)
{
#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		ServerCreateBots();
	}
#endif
}

#if WITH_SERVER_CODE

void UBotCreationComponent::ServerCreateBots_Implementation()
{
	if (BotControllerClass == nullptr)
	{
		return;
	}

	RemainingBotNames = RandomBotNames;

	// Determine how many bots to spawn: use IBotSpawnProvider if present (e.g. from GameFeature_BotSpawn), else NumBotsToCreate
	int32 EffectiveBotCount = NumBotsToCreate;
	if (AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>())
	{
		if (UActorComponent* ProviderComp = GameState->FindComponentByInterface(UBotSpawnProviderInterface::StaticClass()))
		{
			if (IBotSpawnProviderInterface* Provider = Cast<IBotSpawnProviderInterface>(ProviderComp))
			{
				EffectiveBotCount = Provider->GetDesiredBotCount();
			}
		}
	}

	// Create them
	for (int32 Count = 0; Count < EffectiveBotCount; ++Count)
	{
		SpawnOneBot();
	}
}

FString UBotCreationComponent::CreateBotName(int32 PlayerIndex)
{
	FString Result;
	if (RemainingBotNames.Num() > 0)
	{
		const int32 NameIndex = FMath::RandRange(0, RemainingBotNames.Num() - 1);
		Result = RemainingBotNames[NameIndex];
		RemainingBotNames.RemoveAtSwap(NameIndex);
	}
	else
	{
		//@TODO: PlayerId is only being initialized for players right now
		PlayerIndex = FMath::RandRange(260, 260+100);
		Result = FString::Printf(TEXT("Bot %d"), PlayerIndex);
	}
	return Result;
}

#include "Interfaces/ExperienceInterface.h"
#include "Interfaces/PawnExtensionComponentInterface.h"

void UBotCreationComponent::SpawnOneBot()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.OverrideLevel = GetComponentLevel();
	SpawnInfo.ObjectFlags |= RF_Transient;
	AAIController* NewController = GetWorld()->SpawnActor<AAIController>(BotControllerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

	if (NewController != nullptr)
	{
		AGameModeBase* GameMode = GetGameMode<AGameModeBase>();
		check(GameMode);

		if (NewController->PlayerState != nullptr)
		{
			NewController->PlayerState->SetPlayerName(CreateBotName(NewController->PlayerState->GetPlayerId()));
		}

		// Bot controller setup via IGameModeInterface (experience-based init would use GameState's IExperienceManagerInterface if needed)
		if (GameMode->GetClass()->ImplementsInterface(UGameModeInterface::StaticClass()))
		{
			if (IGameModeInterface* GameModeInterface = Cast<IGameModeInterface>(GameMode))
			{
				GameModeInterface->GenericPlayerInitialization(NewController);
			}
		}

		GameMode->RestartPlayer(NewController);

		if (NewController->GetPawn() != nullptr)
		{			
			if (UActorComponent* PawnExtComponent = NewController->GetPawn()->FindComponentByInterface(UPawnExtensionComponentInterface::StaticClass()))
			{
				if (IPawnExtensionComponentInterface* PawnExtInterface = Cast<IPawnExtensionComponentInterface>(PawnExtComponent))
				{
					PawnExtInterface->CheckDefaultInitialization();
				}
			}
		}

		SpawnedBotList.Add(NewController);
	}
}
#include "Interfaces/HealthComponentInterface.h"

void UBotCreationComponent::RemoveOneBot()
{
	if (SpawnedBotList.Num() > 0)
	{
		// Right now this removes a random bot as they're all the same; could prefer to remove one
		// that's high skill or low skill or etc... depending on why you are removing one
		const int32 BotToRemoveIndex = FMath::RandRange(0, SpawnedBotList.Num() - 1);

		AAIController* BotToRemove = SpawnedBotList[BotToRemoveIndex];
		SpawnedBotList.RemoveAtSwap(BotToRemoveIndex);

		if (BotToRemove)
		{
			// If we can find a health component, self-destruct it, otherwise just destroy the actor
			if (APawn* ControlledPawn = BotToRemove->GetPawn())
			{
				if (UActorComponent* PawnExtComponent = ControlledPawn->FindComponentByInterface(UHealthComponentInterface::StaticClass()))
				{
					if(IHealthComponentInterface* HealthInterface = Cast<IHealthComponentInterface>(PawnExtComponent))
					{
						// Note, right now this doesn't work quite as desired: as soon as the player state goes away when
						// the controller is destroyed, the abilities like the death animation will be interrupted immediately
						HealthInterface->DamageSelfDestruct();
					}
				}
				else
				{
					ControlledPawn->Destroy();
				}
			}

			// Destroy the controller (will cause it to Logout, etc...)
			BotToRemove->Destroy();
		}
	}
}

#else // !WITH_SERVER_CODE

void UBotCreationComponent::ServerCreateBots_Implementation()
{
	ensureMsgf(0, TEXT("Bot functions do not exist in client builds!"));
}

void UBotCreationComponent::SpawnOneBot()
{
	ensureMsgf(0, TEXT("Bot functions do not exist in client builds!"));
}

void UBotCreationComponent::RemoveOneBot()
{
	ensureMsgf(0, TEXT("Bot functions do not exist in client builds!"));
}

#endif
