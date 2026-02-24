/*
 * Copyright (c) 2025 Yves Tanas
 * All Rights Reserved.
 *
 * This file is part of the Collections project.
 *
 * Unauthorized copying of this file, via any medium, is strictly prohibited.
 * Proprietary and confidential.
 *
 * This software may be used only as expressly authorized by the copyright holder.
 * Unless required by applicable law or agreed to in writing, software distributed
 * under this license is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 *
 * For licensing inquiries, please contact: yves.tanas@example.com
 *
 * Contributors:
 *   Yves Tanas <yves.tanas@example.com>
 *
 * -------------------------------------------------------------------------------
 * File:        [UnifiedPlayerController.cpp]
 * Created:     [2025-06-12]
 * Description: Asset-aware player controller with modular input and interface registration logic.
 * -------------------------------------------------------------------------------
 */

#include "Controllers/Player/UnifiedPlayerController.h"

#include "DataAssets/Primary/InputConfigDataAssetSet.h"
#include "DataAssets/EnhancedInputMappingContext.h"
#include "Enums/InputSettingType.h"
#include "Input_Settings.h"

#include "Interfaces/Control/PawnControlInterface.h"
#include "Interfaces/Control/CameraControlInterface.h"
#include "Interfaces/Control/InteractControlInterface.h"
#include "Interfaces/Control/HumanoidControlInterface.h"
#include "Interfaces/Control/TurretControlInterface.h"
#include "Interfaces/Control/VehicleControlInterface.h"
#include "Interfaces/Control/HelicopterControlInterface.h"
#include "Interfaces/Control/EngineControlInterface.h"
#include "Interfaces/Control/EngineInputControlInterface.h"
#include "Interfaces/InteractInterface.h"
#include "Interfaces/PossessedByControllerInterface.h"
#include "Interfaces/ScenePersistentPawnInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Logging/Log_Controllers.h"

#include "TimerManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Elemental_Player/Public/States/PlayerState_Elemental.h"
#include "Elemental_Interfaces/Public/Interfaces/AbilitySystemInputInterface.h"
#include "Elemental_Interfaces/Public/Interfaces/CameraAssistInterface.h"
#include "Elemental_Interfaces/Public/Interfaces/LocalPlayerSharedSettingsProviderInterface.h"
#include "Elemental_Interfaces/Public/Interfaces/PawnExtensionComponentInterface.h"
#include "Elemental_Interfaces/Public/Interfaces/SharedSettingsForceFeedbackInterface.h"
#include "Elemental_Interfaces/Public/Interfaces/TeamAgentInterface.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerState.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "GenericPlatform/IInputInterface.h"
#include "Engine/Engine.h"
#include "GameMapsSettings.h"
#include "ReplaySubsystem.h"

AUnifiedPlayerController::AUnifiedPlayerController(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	// Initialize control type to none
	CurrentControlType = EInputSettingType::IST_NONE;
	// Initialize input config data map
	InputConfigDataMap = TMap<EInputSettingType, UInputConfigDataAssetSet*>();
	// Set default view mode
	ViewMode = EViewMode::VM_NONE;

	// Initialize control interfaces
	ComponentInterfaceClasses = {
		UCameraControlInterface::StaticClass(),
		UInteractControlInterface::StaticClass(),
		UHumanoidControlInterface::StaticClass(),
		UTurretControlInterface::StaticClass(),
		UVehicleControlInterface::StaticClass(),
		UHelicopterControlInterface::StaticClass(),
		UEngineControlInterface::StaticClass(),
		UEngineInputControlInterface::StaticClass(),
		UPossessedByControllerInterface::StaticClass(),
		UScenePersistentPawnInterface::StaticClass()
	};

	// Initialize Lyra-style properties
	bForceFeedbackEnabled = true;
	bHideViewTargetPawnNextFrame = false;
	LastSeenPlayerState = nullptr;
}

/**
 * OnPossess (overridden from AController)
 * Triggered when this controller takes control of a pawn.
 * Will delay initialization until all input assets are loaded.
 */
void AUnifiedPlayerController::OnPossess(APawn* PawnToPossess)
{
	UnbindMappingContext(); // Ensure previous state is cleared

	Super::OnPossess(PawnToPossess);
	
	SetIsAutoRunning(false);

	// Wait for all input assets to load before initializing control (as loading may be async)
	if (!AllInputAssetsLoaded())
	{
		UE_LOGFMT(Log_UnifiedController, Display, "{Time}: {Line} {Class}: Waiting for input assets to load before initializing control interface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ControlDataPending);
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_ControlDataPending,
			this,
			&AUnifiedPlayerController::WaitForInputsLoaded,
			0.1f,
			true
		);
		return;
	}

	UE_LOGFMT(Log_UnifiedController, Verbose, "{Time}: {Line} {Class}: All input assets loaded. Initializing control interface.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
	InitializeOnPosses();
}

/**
 * WaitForInputsLoaded
 * Polls until all required input assets are loaded, then calls initialization.
 */
void AUnifiedPlayerController::WaitForInputsLoaded()
{
	if (AllInputAssetsLoaded())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ControlDataPending);
		InitializeOnPosses();
		return;
	}
	UE_LOGFMT(Log_UnifiedController, Display, "{Time}: {Line} {Class}: Waiting for input assets to load before initializing control interface.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
}

/**
 * InitializeOnPosses
 * Handles the main pawn/control interface setup after all input assets have been loaded.
 */
void AUnifiedPlayerController::InitializeOnPosses()
{
	UE_LOGFMT(Log_UnifiedController, Verbose, "{Time}: {Line} {Class}: Initializing control interface on possession.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));

	APawn* PawnToPossess = GetPawn();

	if (!PawnToPossess)
	{
		UE_LOGFMT(Log_UnifiedController, Error, "{Time}: {Line} {Class}: Pawn to possess is null. Cannot initialize control interface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	UClass* PawnClass = PawnToPossess->GetClass();

	if (!PawnClass)
	{
		UE_LOGFMT(Log_UnifiedController, Error, "{Time}: {Line} {Class}: Pawn class is null. Cannot initialize control interface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	// Only continue if the pawn implements a recognized control interface
	if (!PawnClass->ImplementsInterface(UPawnControlInterface::StaticClass()))
	{
		UE_LOGFMT(Log_UnifiedController, Warning, "{Time}: {Line} {Class}: Pawn class {PawnClass} does not implement UPawnControlInterface",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("PawnClass", *PawnClass->GetName()));
		return;
	}

	UpdateSettingToLoad(PawnToPossess);

	if (!InputConfigDataMap.Contains(CurrentControlType))
	{
		UE_LOGFMT(Log_UnifiedController, Log, "{Time}: {Line} {Class}: No input mapping bundle found for current control type {ControlType}. Cannot bind mapping context.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_InputMappingsPending);
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_InputMappingsPending,
			this,
			&AUnifiedPlayerController::WaitForInputMappingsAdded,
			0.1f,
			true
		);
		UE_LOGFMT(Log_UnifiedController, Log, "{Time}: {Line} {Class}: Waiting for input mappings to be added before binding mapping context.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_UnifiedController, Display, "{Time}: {Line} {Class}: Binding mapping context for control type {ControlType}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));

	SetViewTarget(PawnToPossess);

	if (PawnToPossess->GetClass()->ImplementsInterface(UPossessedByControllerInterface::StaticClass()))
	{
		IPossessedByControllerInterface::Execute_ControllerPossessesPawn(PawnToPossess, this);
		UE_LOGFMT(Log_UnifiedController, Verbose, "{Time}: {Line} {Class}: Notified pawn {Pawn} of possession.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("Pawn", *PawnToPossess->GetName()));
	}
	else
	{
		UE_LOGFMT(Log_UnifiedController, Warning, "{Time}: {Line} {Class}: Pawn {Pawn} does not implement UPossessedByControllerInterface. Cannot notify of possession.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("Pawn", *PawnToPossess->GetName()));
	}

	// Diagnostic: summary of bindings added at end of Possess
	{
		const FString ControlTypeStr = UEnum::GetValueAsString(CurrentControlType);
		const FString ContextsList = ActiveMappingContextNames.Num() > 0
			? FString::Join(ActiveMappingContextNames, TEXT(", "))
			: TEXT("(none)");
		UE_LOGFMT(Log_UnifiedController, Log, "========== Possess complete ==========");
		UE_LOGFMT(Log_UnifiedController, Log, "  Control type: {ControlType}",
			("ControlType", ControlTypeStr));
		UE_LOGFMT(Log_UnifiedController, Log, "  Pawn: {PawnName}",
			("PawnName", *PawnToPossess->GetName()));
		UE_LOGFMT(Log_UnifiedController, Log, "  Mapping contexts added: {Contexts}",
			("Contexts", ContextsList));
		UE_LOGFMT(Log_UnifiedController, Log, "======================================");
	}
}

/**
 * WaitForInputMappingsAdded
 * Polls until valid input mappings are available, then binds the mapping context.
 */
void AUnifiedPlayerController::WaitForInputMappingsAdded()
{
	if (!InputConfigDataMap.Contains(CurrentControlType))
	{
		UE_LOGFMT(Log_UnifiedController, Verbose, "{Time}: {Line} {Class}: No input mapping bundle found for current control type {ControlType}. Waiting for input mappings to be added.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));
		return;
	}
	
	UInputConfigDataAssetSet* InputConfigDataAsset = InputConfigDataMap[CurrentControlType];
	if(!InputConfigDataAsset)
	{
		UE_LOGFMT(Log_UnifiedController, Error, "{Time}: {Line} {Class}: InputConfigDataAssetSet for current control type {ControlType} is null. Cannot bind mapping context.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));
		return;
	}

	for (auto MappingContextPtr : InputConfigDataAsset->InputMappingContexts)
	{

		if (!MappingContextPtr.IsValid())
		{
			UE_LOGFMT(Log_UnifiedController, Warning, "{Time}: {Line} {Class}: Input mapping context is not valid. Waiting for valid input mappings.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
			return;
		}

		UEnhancedInputMappingContext* MappingContext = MappingContextPtr.Get();

		if (!IsValid(MappingContext))
		{
			UE_LOGFMT(Log_UnifiedController, Warning, "{Time}: {Line} {Class}: Input mapping context {MappingContext} is not valid for current control type {ControlType}. Waiting for input mappings to be added.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
				("MappingContext", *MappingContext->GetName()), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));
			return;
		}

		if (MappingContext->InputMappingType == EInputMappingType::IMT_DEFAULT)
		{
			UE_LOGFMT(Log_UnifiedController, Verbose, "{Time}: {Line} {Class}: Found invalid input mapping context {MappingContext} for current default control type {ControlType}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
				("MappingContext", *MappingContext->GetName()), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));
			continue;
		}

		SetupEnhancedInputMappingContext(MappingContext);
		UE_LOGFMT(Log_UnifiedController, Log, "{Time}: {Line} {Class}: Successfully bound mapping context {MappingContext} for control type {ControlType}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("MappingContext", *MappingContext->GetName()), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));

	}
}

/**
 * UpdateSettingToLoad
 * Determines which input configuration should be loaded for a given pawn (based on supported interfaces).
 */
bool AUnifiedPlayerController::UpdateSettingToLoad(APawn* PawnToPossess)
{
	if (!PawnToPossess)
	{
		UE_LOGFMT(Log_UnifiedController, Warning, "{Time}: {Line} {Class}: Current pawn is null. Cannot update setting to load.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return false;
	}

	if (PawnToPossess->GetClass()->ImplementsInterface(UPawnControlInterface::StaticClass()))
	{
		EPawnControlType ControlType = IPawnControlInterface::Execute_GetPawnControlType(PawnToPossess);

		switch (ControlType)
		{
		case EPawnControlType::PCT_NONE:
			return false;
			break;
		case EPawnControlType::PCT_HUMANOID:
			CurrentControlType = EInputSettingType::IST_HUMANOID;
			return true;
			break;
		case EPawnControlType::PCT_VEHICLE:
			CurrentControlType = EInputSettingType::IST_VEHICLE;
			return true;
			break;
		case EPawnControlType::PCT_AIRCRAFT:
			CurrentControlType = EInputSettingType::IST_AIRCRAFT;
			return true;
			break;
		case EPawnControlType::PCT_TURRET:
			CurrentControlType = EInputSettingType::IST_TURRET;
			return true;
			break;
		case EPawnControlType::PCT_HELICOPTER:
			CurrentControlType = EInputSettingType::IST_HELICOPTER;
			return true;
			break;
		default:
			break;
		}
	}
	else
	{
		UE_LOGFMT(Log_UnifiedController, Warning, "{Time}: {Line} {Class}: Pawn class {PawnClass} does not implement UPawnControlInterface. Cannot determine control type.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("PawnClass", *PawnToPossess->GetClass()->GetName()));
	}

	return false;
}

/**
 * UnbindMappingContext
 * Clears the input system, removes all mapping contexts and action bindings, resets internal state and unregisters components.
 * Always clears Enhanced Input mappings on unpossess, even if no configs were loaded.
 */
bool AUnifiedPlayerController::UnbindMappingContext()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_ControlDataPending))
	{
		UE_LOGFMT(Log_UnifiedController, Warning, "{Time}: {Line} {Class}: Clearing pending control data timer.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ControlDataPending);
	}
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_InputMappingsPending))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_InputMappingsPending);
	}

	const int32 NumMappingContextsRemoved = ActiveMappingContextNames.Num();
	const int32 NumRegisteredComponentsCleared = RegisteredComponentsByClass.Num();

	// Reset the controller�s state (including control type, registered components, load counters, etc.)
	CurrentControlType = EInputSettingType::IST_NONE;

	UE_LOGFMT(Log_UnifiedController, Verbose, "{Time}: {Line} {Class}: Unbound current control interface and cleared state.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));

	RegisteredComponentsByClass.Empty();
	UE_LOGFMT(Log_UnifiedController, Verbose, "{Time}: {Line} {Class}: Cleared all registered components: {Num}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("Num", NumRegisteredComponentsCleared));

	InputCount = 0;

	UE_LOGFMT(Log_UnifiedController, Verbose, "{Time}: {Line} {Class}: Cleared all registered components.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));

	// Remove input mapping contexts from the subsystem
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!InputSubsystem)
	{
		UE_LOGFMT(Log_UnifiedController, Error, "{Time}: {Line} {Class}: EnhancedInputLocalPlayerSubsystem not found! Cannot unbind mapping context.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return false;
	}

	// Diagnostic: summary of bindings removed at UnPossess
	{
		const FString ContextsList = NumMappingContextsRemoved > 0
			? FString::Join(ActiveMappingContextNames, TEXT(", "))
			: TEXT("(none)");
		UE_LOGFMT(Log_UnifiedController, Log, "========== UnPossess complete ==========");
		UE_LOGFMT(Log_UnifiedController, Log, "  Mapping contexts removed: {Contexts}",
			("Contexts", ContextsList));
		UE_LOGFMT(Log_UnifiedController, Log, "  Registered components cleared: {Num}",
			("Num", NumRegisteredComponentsCleared));
		UE_LOGFMT(Log_UnifiedController, Log, "=========================================");
	}
	ActiveMappingContextNames.Empty();

	// Empty mapping context list and clear from input subsystem
	UE_LOGFMT(Log_UnifiedController, Verbose, "{Time}: {Line} {Class}: Clearing all active mapping contexts for current control type {ControlType}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("ControlType", UEnum::GetValueAsString(CurrentControlType)));
	
	InputSubsystem->ClearAllMappings();

	return true;
}

//~ Lyra-style functionality (Elemental interfaces only)

namespace
{
	const FName StatusTag_AutoRunning(TEXT("Status.AutoRunning"));
}

AHUD* AUnifiedPlayerController::GetWarCollectionHUD() const
{
	return GetHUD();
}

UAbilitySystemComponent* AUnifiedPlayerController::GetAbilitySystemComponent() const
{
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(PlayerState))
	{
		return ASI->GetAbilitySystemComponent();
	}
	return nullptr;
}

void AUnifiedPlayerController::SetIsAutoRunning(const bool bEnabled)
{
	const bool bCurrentAutoRunning = GetIsAutoRunning();
	if (bEnabled != bCurrentAutoRunning)
	{
		if (!bEnabled)
		{
			OnEndAutoRun();
		}
		else
		{
			OnStartAutoRun();
		}
	}
}

bool AUnifiedPlayerController::GetIsAutoRunning() const
{
	bool bCurrentAutoRunning = false;
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (IAbilitySystemInputInterface* InputI = Cast<IAbilitySystemInputInterface>(ASC))
		{
			const FGameplayTag AutoRunTag = FGameplayTag::RequestGameplayTag(StatusTag_AutoRunning);
			bCurrentAutoRunning = InputI->GetTagCount(AutoRunTag) > 0;
		}
	}
	return bCurrentAutoRunning;
}

void AUnifiedPlayerController::OnStartAutoRun()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (IAbilitySystemInputInterface* InputI = Cast<IAbilitySystemInputInterface>(ASC))
		{
			const FGameplayTag AutoRunTag = FGameplayTag::RequestGameplayTag(StatusTag_AutoRunning);
			InputI->SetLooseGameplayTagCount(AutoRunTag, 1);
		}
		K2_OnStartAutoRun();
	}
}

void AUnifiedPlayerController::OnEndAutoRun()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (IAbilitySystemInputInterface* InputI = Cast<IAbilitySystemInputInterface>(ASC))
		{
			const FGameplayTag AutoRunTag = FGameplayTag::RequestGameplayTag(StatusTag_AutoRunning);
			InputI->SetLooseGameplayTagCount(AutoRunTag, 0);
		}
		K2_OnEndAutoRun();
	}
}

void AUnifiedPlayerController::ServerCheat_Implementation(const FString& Msg)
{
	if (PlayerState != nullptr)
	{
		// Execute cheat command
		// TODO: Implement cheat manager if needed
		UE_LOG(LogTemp, Warning, TEXT("ServerCheat: %s from %s"), *Msg, *GetNameSafe(PlayerState));
	}
}

bool AUnifiedPlayerController::ServerCheat_Validate(const FString& Msg)
{
	return true;
}

void AUnifiedPlayerController::ServerCheatAll_Implementation(const FString& Msg)
{
	// Execute cheat command for all players
	// TODO: Implement cheat manager if needed
	UE_LOG(LogTemp, Warning, TEXT("ServerCheatAll: %s"), *Msg);
}

bool AUnifiedPlayerController::ServerCheatAll_Validate(const FString& Msg)
{
	return true;
}

//~AActor interface
void AUnifiedPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AUnifiedPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(false);
}

void AUnifiedPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AUnifiedPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Disable replicating the PC target view as it doesn't work well for replays or client-side spectating.
	// The engine TargetViewRotation is only set in APlayerController::TickActor if the server knows ahead of time that 
	// a specific pawn is being spectated and it only replicates down for COND_OwnerOnly.
	// In client-saved replays, COND_OwnerOnly is never true and the target pawn is not always known at the time of recording.
	// To support client-saved replays, the replication of this was moved to ReplicatedViewRotation and updated in PlayerTick.
	DISABLE_REPLICATED_PROPERTY(APlayerController, TargetViewRotation);
}
//~End of AActor interface

//~AController interface
void AUnifiedPlayerController::OnUnPossess()
{
	// 1) Clear ASC avatar for the pawn being unpossessed (via Elemental interface only)
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		for (UActorComponent* Comp : PawnBeingUnpossessed->GetComponents())
		{
			if (Comp && Comp->GetClass()->ImplementsInterface(UPawnExtensionComponentInterface::StaticClass()))
			{
				if (UAbilitySystemComponent* ASC = IPawnExtensionComponentInterface::Execute_GetAbilitySystemComponent(Comp))
				{
					if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
					{
						IPawnExtensionComponentInterface::Execute_UninitializeAbilitySystem(Comp);
					}
				}
				break;
			}
		}
	}

	// 2) Unbind mapping context and notify pawn (scene persistent)
	UnbindMappingContext();
	APawn* CurrentPawn = GetPawn();
	if (CurrentPawn)
	{
		if (CurrentPawn->GetClass()->ImplementsInterface(UScenePersistentPawnInterface::StaticClass()))
		{
			if (IScenePersistentPawnInterface::Execute_ShouldDespawnOnUnpossess(CurrentPawn))
			{
				CurrentPawn->SetLifeSpan(0.01f);
			}
		}
	}

	Super::OnUnPossess();
}

void AUnifiedPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AUnifiedPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AUnifiedPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BroadcastOnPlayerStateChanged();

	// When we're a client, retry activating on-spawn abilities once PlayerState is replicated (via IAbilitySystemInterface).
	if (GetWorld()->IsNetMode(NM_Client))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		{
			ASC->RefreshAbilityActorInfo();
			if (IAbilitySystemInputInterface* InputI = Cast<IAbilitySystemInputInterface>(ASC))
			{
				InputI->TryActivateAbilitiesOnSpawn();
			}
		}
	}
}
//~End of AController interface

//~APlayerController interface
void AUnifiedPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
}

void AUnifiedPlayerController::SetPlayer(UPlayer* InPlayer)
{
	Super::SetPlayer(InPlayer);

	if (ILocalPlayerSharedSettingsProviderInterface* LP = Cast<ILocalPlayerSharedSettingsProviderInterface>(InPlayer))
	{
		if (UObject* UserSettings = LP->GetSharedSettings())
		{
			if (ISharedSettingsForceFeedbackInterface* SettingsInterface = Cast<ISharedSettingsForceFeedbackInterface>(UserSettings))
			{
				SettingsInterface->AddSettingChangedListener(this, FName(TEXT("OnSettingsChanged")));
				OnSettingsChanged(UserSettings);
			}
		}
	}
}

void AUnifiedPlayerController::AddCheats(bool bForce)
{
#if USING_CHEAT_MANAGER
	Super::AddCheats(true);
#else
	Super::AddCheats(bForce);
#endif
}

void AUnifiedPlayerController::UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId)
{
	if (bForceFeedbackEnabled)
	{
		if (const UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(GetLocalPlayer()))
		{
			const ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();
			if (CurrentInputType == ECommonInputType::Gamepad || CurrentInputType == ECommonInputType::Touch)
			{
				InputInterface->SetForceFeedbackChannelValues(ControllerId, ForceFeedbackValues);
				return;
			}
		}
	}
	
	InputInterface->SetForceFeedbackChannelValues(ControllerId, FForceFeedbackValues());
}

void AUnifiedPlayerController::UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& OutHiddenComponents)
{
	Super::UpdateHiddenComponents(ViewLocation, OutHiddenComponents);

	if (bHideViewTargetPawnNextFrame)
	{
		AActor* const ViewTargetPawn = PlayerCameraManager ? Cast<AActor>(PlayerCameraManager->GetViewTarget()) : nullptr;
		if (ViewTargetPawn)
		{
			// internal helper func to hide all the components
			auto AddToHiddenComponents = [&OutHiddenComponents](const TInlineComponentArray<UPrimitiveComponent*>& InComponents)
			{
				// add every component and all attached children
				for (UPrimitiveComponent* Comp : InComponents)
				{
					if (Comp->IsRegistered())
					{
						OutHiddenComponents.Add(Comp->GetPrimitiveSceneId());

						for (USceneComponent* AttachedChild : Comp->GetAttachChildren())
						{
							static FName NAME_NoParentAutoHide(TEXT("NoParentAutoHide"));
							UPrimitiveComponent* AttachChildPC = Cast<UPrimitiveComponent>(AttachedChild);
							if (AttachChildPC && AttachChildPC->IsRegistered() && !AttachChildPC->ComponentTags.Contains(NAME_NoParentAutoHide))
							{
								OutHiddenComponents.Add(AttachChildPC->GetPrimitiveSceneId());
							}
						}
					}
				}
			};

			// hide pawn's components
			TInlineComponentArray<UPrimitiveComponent*> PawnComponents;
			ViewTargetPawn->GetComponents(PawnComponents);
			AddToHiddenComponents(PawnComponents);
		}

		// we consumed it, reset for next frame
		bHideViewTargetPawnNextFrame = false;
	}
}

void AUnifiedPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void AUnifiedPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (IAbilitySystemInputInterface* InputI = Cast<IAbilitySystemInputInterface>(ASC))
		{
			InputI->ProcessAbilityInput(DeltaTime, bGamePaused);
		}
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AUnifiedPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// If we are auto running then add some player input
	if (GetIsAutoRunning())
	{
		if (APawn* CurrentPawn = GetPawn())
		{
			const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			CurrentPawn->AddMovementInput(MovementDirection, 1.0f);	
		}
	}

	APlayerState_Elemental* ElementalPlayerState = GetPlayerState<APlayerState_Elemental>();

	if (PlayerCameraManager && ElementalPlayerState)
	{
		APawn* TargetPawn = PlayerCameraManager->GetViewTargetPawn();

		if (TargetPawn)
		{
			// Update view rotation on the server so it replicates
			if (HasAuthority() || TargetPawn->IsLocallyControlled())
			{
				ElementalPlayerState->SetReplicatedViewRotation(TargetPawn->GetViewRotation());
			}

			// Update the target view rotation if the pawn isn't locally controlled
			if (!TargetPawn->IsLocallyControlled())
			{
				ElementalPlayerState = TargetPawn->GetPlayerState<APlayerState_Elemental>();
				if (ElementalPlayerState)
				{
					// Get it from the spectated pawn's player state, which may not be the same as the PC's playerstate
					TargetViewRotation = ElementalPlayerState->GetReplicatedViewRotation();
				}
			}
		}
	}
}
//~End of APlayerController interface

//~ICameraAssistInterface interface
void AUnifiedPlayerController::OnCameraPenetratingTarget()
{
	bHideViewTargetPawnNextFrame = true;
}
//~End of ICameraAssistInterface interface

//~ITeamAgentInterface interface
void AUnifiedPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a player controller (%s); it's driven by the associated player state"), *GetPathNameSafe(this));
}

FGenericTeamId AUnifiedPlayerController::GetGenericTeamId() const
{
	if (const ITeamAgentInterface* PSWithTeamInterface = Cast<ITeamAgentInterface>(PlayerState))
	{
		return PSWithTeamInterface->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

FOnTeamIndexChangedDelegate* AUnifiedPlayerController::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}
//~End of ITeamAgentInterface interface

//~Protected methods
void AUnifiedPlayerController::OnPlayerStateChanged()
{
	// Empty, place for derived classes to implement without having to hook all the other events
}

void AUnifiedPlayerController::OnSettingsChanged(UObject* Settings)
{
	if (ISharedSettingsForceFeedbackInterface* SettingsI = Cast<ISharedSettingsForceFeedbackInterface>(Settings))
	{
		bForceFeedbackEnabled = SettingsI->GetForceFeedbackEnabled();
	}
}

bool AUnifiedPlayerController::TryToRecordClientReplay()
{
	// See if we should record a replay
	if (ShouldRecordClientReplay())
	{
		if (UReplaySubsystem* ReplaySubsystem = GetGameInstance()->GetSubsystem<UReplaySubsystem>())
		{
			APlayerController* FirstLocalPlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (FirstLocalPlayerController == this)
			{
				// If this is the first player, start recording. TODO: UE 5.6 ReplaySubsystem API may differ (e.g. RecordReplay); re-enable with correct method.
				// ReplaySubsystem->StartRecordingReplay(GetWorld(), TEXT("ClientReplay"), TEXT(""));
			}
		}
	}
	return false;
}

bool AUnifiedPlayerController::ShouldRecordClientReplay()
{
	UWorld* World = GetWorld();
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance != nullptr &&
		World != nullptr &&
		!World->IsPlayingReplay() &&
		!World->IsRecordingClientReplay() &&
		NM_DedicatedServer != GetNetMode() &&
		IsLocalPlayerController())
	{
		FString DefaultMap = UGameMapsSettings::GetGameDefaultMap();
		FString CurrentMap = World->URL.Map;

#if WITH_EDITOR
		CurrentMap = UWorld::StripPIEPrefixFromPackageName(CurrentMap, World->StreamingLevelsPrefix);
#endif
		if (CurrentMap == DefaultMap)
		{
			// Never record demos on the default frontend map, this could be replaced with a better check for being in the main menu
			return false;
		}

		if (UReplaySubsystem* ReplaySubsystem = GameInstance->GetSubsystem<UReplaySubsystem>())
		{
			if (ReplaySubsystem->IsRecording() || ReplaySubsystem->IsPlaying())
			{
				// Only one at a time
				return false;
			}
		}

		// If local player provides shared settings, game can extend replay logic via interface
		if (GetLocalPlayer() && GetLocalPlayer()->GetClass()->ImplementsInterface(ULocalPlayerSharedSettingsProviderInterface::StaticClass()))
		{
			// Optional: check settings for auto-record via ISharedSettingsForceFeedbackInterface or future interface
		}
	}
	return false;
}

void AUnifiedPlayerController::BroadcastOnPlayerStateChanged()
{
	OnPlayerStateChanged();

	// Unbind from the old player state, if any
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (LastSeenPlayerState != nullptr)
	{
		if (ITeamAgentInterface* PlayerStateTeamInterface = Cast<ITeamAgentInterface>(LastSeenPlayerState))
		{
			OldTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetOnTeamIndexChangedDelegate()->RemoveAll(this);
		}
	}

	// Bind to the new player state, if any
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (PlayerState != nullptr)
	{
		if (ITeamAgentInterface* PlayerStateTeamInterface = Cast<ITeamAgentInterface>(PlayerState))
		{
			NewTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetOnTeamIndexChangedDelegate()->AddDynamic(this, &ThisClass::OnPlayerStateChangedTeam);
		}
	}

	// Broadcast the team change (if it really has)
	ITeamAgentInterface::ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);

	LastSeenPlayerState = PlayerState;
}

void AUnifiedPlayerController::OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	ITeamAgentInterface::ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
}
