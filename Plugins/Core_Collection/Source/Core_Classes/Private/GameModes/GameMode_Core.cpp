// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/GameMode_Core.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "GameFramework/DefaultPawn.h"
#include "Logging/LogMacros.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "TimerManager.h"
#include "Misc/CommandLine.h"
#include "Kismet/GameplayStatics.h"
#include "AssetRegistry/AssetData.h"

#include "Subsystems/PawnDataProviderSubsystem.h"
#include "Interfaces/PawnDataPawnClassProviderInterface.h"
#include "GameModes/CoreGameState.h"
#include "Interfaces/ExperienceManagerInterface.h"
#include "Elemental_Interfaces/Public/Interfaces/PawnExtensionComponentInterface.h"
#include "Elemental_Common/Public/Subsystems/ElementalInitStateTagSubsystem.h"
#include "GameplayTags_Core.h"
#include "GameMapsSettings.h"

DEFINE_LOG_CATEGORY(Log_GameMode_Core);

AGameMode_Core::AGameMode_Core(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ACoreGameState::StaticClass();
}

const UObject* AGameMode_Core::GetPawnDataForController(AController* Controller) const
{
	return GetPawnDataForControllerImpl(Controller);
}

const UObject* AGameMode_Core::GetPawnDataForControllerImpl(const AController* InController) const
{
	if (UPawnDataProviderSubsystem* Subsystem = GetWorld() ? GetWorld()->GetSubsystem<UPawnDataProviderSubsystem>() : nullptr)
	{
		return Subsystem->GetPawnDataForController(InController);
	}
	return nullptr;
}

void AGameMode_Core::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UWorld* World = GetWorld();
	if (World)
	{
		if (UElementalInitStateTagSubsystem* InitStateTags = World->GetSubsystem<UElementalInitStateTagSubsystem>())
		{
			InitStateTags->SetInitStateTags(
				GameplayTags_Core::InitState_Spawned,
				GameplayTags_Core::InitState_DataAvailable,
				GameplayTags_Core::InitState_DataInitialized,
				GameplayTags_Core::InitState_GameplayReady);
			InitStateTags->SetPawnExtensionFeatureName(FName("PawnExtension"));
		}
	}

	// Wait for the next frame to give time to initialize startup settings
	if (World)
	{
		World->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
	}
}

void AGameMode_Core::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	FString ExperienceIdSource;

	// Precedence order (highest wins)
	//  - Matchmaking assignment (if present)
	//  - URL Options override
	//  - Developer Settings (PIE only)
	//  - Command Line override
	//  - World Settings
	//  - Dedicated server
	//  - Default experience

	UWorld* World = GetWorld();
	const FString UrlOptions = World->URL.Op.Num() > 0 ? FString::Join(World->URL.Op, TEXT("?")) : FString();

	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(UrlOptions, TEXT("Experience")))
	{
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(UrlOptions, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(TEXT("ExperienceDefinition")), FName(*ExperienceFromOptions));
		ExperienceIdSource = TEXT("OptionsString");
	}

	// see if the command line wants to set the experience
	if (!ExperienceId.IsValid())
	{
		FString ExperienceFromCommandLine;
		if (FParse::Value(FCommandLine::Get(), TEXT("Experience="), ExperienceFromCommandLine))
		{
			ExperienceId = FPrimaryAssetId::ParseTypeAndName(ExperienceFromCommandLine);
			if (!ExperienceId.PrimaryAssetType.IsValid())
			{
				ExperienceId = FPrimaryAssetId(FPrimaryAssetType(TEXT("ExperienceDefinition")), FName(*ExperienceFromCommandLine));
			}
			ExperienceIdSource = TEXT("CommandLine");
		}
	}

	FAssetData Dummy;
	UPawnDataProviderSubsystem* PawnDataProvider = GetWorld() ? GetWorld()->GetSubsystem<UPawnDataProviderSubsystem>() : nullptr;
	if (ExperienceId.IsValid() && (!PawnDataProvider || !PawnDataProvider->GetPrimaryAssetData(ExperienceId, /*out*/ Dummy)))
	{
		UE_LOG(Log_GameMode_Core, Error, TEXT("EXPERIENCE: Wanted to use %s but couldn't find it, falling back to the default"), *ExperienceId.ToString());
		ExperienceId = FPrimaryAssetId();
	}

	// Final fallback to the default experience
	if (!ExperienceId.IsValid())
	{
		//@TODO: Pull this from a config setting or something
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("ExperienceDefinition"), FName("B_DefaultExperience")); // Rename content asset from B_WarCollectionDefaultExperience if needed
		ExperienceIdSource = TEXT("Default");
	}

	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}

void AGameMode_Core::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource)
{
	if (ExperienceId.IsValid())
	{
		UE_LOG(Log_GameMode_Core, Log, TEXT("Identified experience %s (Source: %s)"), *ExperienceId.ToString(), *ExperienceIdSource);

		if (IExperienceManagerInterface* ExperienceInterface = Cast<IExperienceManagerInterface>(GameState->FindComponentByInterface(UExperienceManagerInterface::StaticClass())))
		{
			ExperienceInterface->SetCurrentExperience(ExperienceId);
		}
		else
		{
			UE_LOG(Log_GameMode_Core, Error, TEXT("GameState has no IExperienceManagerInterface component, cannot set experience"));
		}
	}
	else
	{
		UE_LOG(Log_GameMode_Core, Error, TEXT("Failed to identify experience, loading screen will stay up forever"));
	}
}

void AGameMode_Core::OnExperienceLoaded(const UExperienceDefinition* CurrentExperience)
{
	// Spawn any players that are already attached
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

bool AGameMode_Core::IsExperienceLoaded() const
{
	check(GameState);
	if (IExperienceManagerInterface* ExperienceInterface = Cast<IExperienceManagerInterface>(GameState->FindComponentByInterface(UExperienceManagerInterface::StaticClass())))
	{
		return ExperienceInterface->IsExperienceLoaded();
	}
	return false;
}

UClass* AGameMode_Core::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UObject* PawnData = GetPawnDataForControllerImpl(InController))
	{
		if (IPawnDataPawnClassProviderInterface* Provider = Cast<IPawnDataPawnClassProviderInterface>(const_cast<UObject*>(PawnData)))
		{
			if (UClass* PawnClass = Provider->GetPawnClass())
			{
				return PawnClass;
			}
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* AGameMode_Core::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (UActorComponent* PawnExtComp = SpawnedPawn->FindComponentByInterface(UPawnExtensionComponentInterface::StaticClass()))
			{
				if (UObject* PawnData = const_cast<UObject*>(GetPawnDataForControllerImpl(NewPlayer)))
				{
					IPawnExtensionComponentInterface::Execute_SetPawnData(PawnExtComp, PawnData);
				}
				else
				{
					UE_LOG(Log_GameMode_Core, Error, TEXT("Game mode was unable to set PawnData on the spawned pawn [%s]."), *GetNameSafe(SpawnedPawn));
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);

			return SpawnedPawn;
		}
		else
		{
			UE_LOG(Log_GameMode_Core, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(Log_GameMode_Core, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}

	return nullptr;
}

bool AGameMode_Core::ShouldSpawnAtStartSpot(AController* Player)
{
	// We never want to use the start spot, always use the spawn management component.
	return false;
}

void AGameMode_Core::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Delay starting new players until the experience has been loaded
	// (players who log in prior to that will be started by OnExperienceLoaded)
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

AActor* AGameMode_Core::ChoosePlayerStart_Implementation(AController* Player)
{
	// TODO: Implement PlayerSpawningManagerComponent if needed
	// if (UWarCollectionPlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<UWarCollectionPlayerSpawningManagerComponent>())
	// {
	// 	return PlayerSpawningComponent->ChoosePlayerStart(Player);
	// }
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

void AGameMode_Core::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	// TODO: Implement PlayerSpawningManagerComponent if needed
	// if (UWarCollectionPlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<UWarCollectionPlayerSpawningManagerComponent>())
	// {
	// 	PlayerSpawningComponent->FinishRestartPlayer(NewPlayer, StartRotation);
	// }

	Super::FinishRestartPlayer(NewPlayer, StartRotation);
}

bool AGameMode_Core::PlayerCanRestart_Implementation(APlayerController* Player)
{
	return ControllerCanRestart(Player);
}

bool AGameMode_Core::ControllerCanRestart(AController* Controller)
{
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{	
		if (!Super::PlayerCanRestart_Implementation(PC))
		{
			return false;
		}
	}
	else
	{
		// Bot version of Super::PlayerCanRestart_Implementation
		if ((Controller == nullptr) || Controller->IsPendingKillPending())
		{
			return false;
		}
	}

	// TODO: Implement PlayerSpawningManagerComponent if needed
	// if (UWarCollectionPlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<UWarCollectionPlayerSpawningManagerComponent>())
	// {
	// 	return PlayerSpawningComponent->ControllerCanRestart(Controller);
	// }

	return true;
}

void AGameMode_Core::InitGameState()
{
	Super::InitGameState();

	// Listen for the experience load to complete
	if (IExperienceManagerInterface* ExperienceInterface = Cast<IExperienceManagerInterface>(GameState->FindComponentByInterface(UExperienceManagerInterface::StaticClass())))
	{
		ExperienceInterface->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void AGameMode_Core::GenericPlayerInitialization(AController* NewPlayer)
{
	Super::GenericPlayerInitialization(NewPlayer);

	OnGameModePlayerInitialized.Broadcast(this, NewPlayer);
}

void AGameMode_Core::RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset)
{
	if (bForceReset && (Controller != nullptr))
	{
		Controller->Reset();
	}

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(PC, &APlayerController::ServerRestartPlayer_Implementation);
	}
	// TODO: Implement Bot Controller restart if needed
	// else if (AWarCollectionPlayerBotController* BotController = Cast<AWarCollectionPlayerBotController>(Controller))
	// {
	// 	GetWorldTimerManager().SetTimerForNextTick(BotController, &AWarCollectionPlayerBotController::ServerRestartController);
	// }
}

bool AGameMode_Core::UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage)
{
	// Do nothing, we'll wait until PostLogin when we try to spawn the player for real.
	// Doing anything right now is no good, systems like team assignment haven't even occurred yet.
	return true;
}

void AGameMode_Core::FailedToRestartPlayer(AController* NewPlayer)
{
	Super::FailedToRestartPlayer(NewPlayer);

	// If we tried to spawn a pawn and it failed, lets try again *note* check if there's actually a pawn class
	// before we try this forever.
	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APlayerController* NewPC = Cast<APlayerController>(NewPlayer))
		{
			// If it's a player don't loop forever, maybe something changed and they can no longer restart if so stop trying.
			if (PlayerCanRestart(NewPC))
			{
				RequestPlayerRestartNextFrame(NewPlayer, false);			
			}
			else
			{
				UE_LOG(Log_GameMode_Core, Verbose, TEXT("FailedToRestartPlayer(%s) and PlayerCanRestart returned false, so we're not going to try again."), *GetPathNameSafe(NewPlayer));
			}
		}
		else
		{
			RequestPlayerRestartNextFrame(NewPlayer, false);
		}
	}
	else
	{
		UE_LOG(Log_GameMode_Core, Verbose, TEXT("FailedToRestartPlayer(%s) but there's no pawn class so giving up."), *GetPathNameSafe(NewPlayer));
	}
}

void AGameMode_Core::BeginPlay()
{
	Super::BeginPlay();
	UE_LOGFMT(Log_GameMode_Core, Display, "{Time}: {Line} { Class } : GameMode Core started.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
}

void AGameMode_Core::SpawnNewActorAtLocation_Implementation(AController* InInstigator, const FTransform& InTransform)
{
	if (InInstigator == nullptr)
	{
		UE_LOGFMT(Log_GameMode_Core, Warning, "{Time}: {Line} {Class}: Instigator is null. Cannot spawn new actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = InInstigator->GetPawn();
	SpawnParams.Owner = InInstigator;
	AActor* NewActor = GetWorld()->SpawnActor<AActor>(GetDefaultPawnClassForController(InInstigator), InTransform, SpawnParams);
	if (NewActor)
	{
		UE_LOGFMT(Log_GameMode_Core, Display, "{Time}: {Line} {Class}: Successfully spawned new actor {ActorName} at location {Location}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActorName", *NewActor->GetName()), ("Location", InTransform.GetLocation().ToString()));
		if (InInstigator->GetPawn())
		{
			InInstigator->GetPawn()->SetActorLocation(InTransform.GetLocation());
			InInstigator->GetPawn()->SetActorRotation(InTransform.GetRotation());
		}
	}
	else
	{
		UE_LOGFMT(Log_GameMode_Core, Error, "{Time}: {Line} {Class}: Failed to spawn new actor at location {Location}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("Location", InTransform.GetLocation().ToString()));
	}
}
