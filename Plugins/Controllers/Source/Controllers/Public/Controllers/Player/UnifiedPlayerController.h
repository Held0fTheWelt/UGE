/*
 * Copyright (c) 2024 Yves Tanas
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
 * For licensing inquiries, please contact: Yves.Tanas@gmx.de
 *
 * Contributors:
 *   Yves Tanas <Yves.Tanas@gmx.de>
 *
 * -------------------------------------------------------------------------------
 * File:        [UnifiedPlayerController.h]
 * Created:     [2025-06-12]
 * Description: Unified PlayerController with dynamic input, pawn, vehicle and humanoid support, including component registration.
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "Controllers/Player/InputLoadingPlayerController.h"
#include "Enums/InputConfiguration.h"
#include "Enums/InputSettingType.h"
#include "Enums/ViewMode.h"
#include "Interfaces/PlayerControllerInterface.h"
#include "Interfaces/RegisterComponentInterface.h"
#include "Interfaces/CameraAssistInterface.h"
#include "Interfaces/TeamAgentInterface.h"
#include "InputActionValue.h"
#include "TimerManager.h"
#include "UnifiedPlayerController.generated.h"

 // Main log categories for this controller


/**
 * AUnifiedPlayerController
 *
 * Unified player controller supporting enhanced input, pawn, humanoid, and vehicle controls,
 * with runtime dynamic asset loading and component interface registration.
 */
UCLASS()
class CONTROLLERS_API AUnifiedPlayerController : public AInputLoadingPlayerController, 
	public IRegisterComponentInterface, 
	public IPlayerControllerInterface,
	public ICameraAssistInterface,
	public ITeamAgentInterface
{
	GENERATED_BODY()

public:
	AUnifiedPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	/** Current camera/view mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	EViewMode ViewMode = EViewMode::VM_NONE;

	/** Pawn load index (for respawn or input remapping) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Load")
	int32 PlayerPawnLoadIndexOnPawn = -1;

	UPROPERTY(Transient)
	int32 InputCount = 0;

protected:
	/** Called when a pawn is possessed by this controller */
	virtual void OnPossess(APawn* PawnToPossess) override;

	/** Called when this controller loses its pawn */
	virtual void OnUnPossess() override;

	/** Update input settings based on the new pawn */
	virtual bool UpdateSettingToLoad(APawn* PawnToPossess) override;

private:
	/** Wait until all input assets are loaded */
	void WaitForInputsLoaded();

	/** Wait until all input mapping contexts are added */
	void WaitForInputMappingsAdded();

	/** Called during OnPossess after all async/asset loading is done */
	void InitializeOnPosses();

	/** Remove current input mapping context bindings */
	bool UnbindMappingContext();

	/** Called when an enhanced input action asset has finished loading */
	virtual void SetupEnhancedInputAction(class UEnhancedInputAction* InputAction) override;

private:
	/** Handles for asynchronous/timed initialization steps */
	FTimerHandle TimerHandle_ControlDataPending;
	FTimerHandle TimerHandle_InputMappingsPending;

protected:
	/** List of component interface classes eligible for registration by this controller */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Registering Interface")
	TArray<UClass*> ComponentInterfaceClasses;

private:
	/** Register a component with its respective interface (required by IRegisterComponentInterface) */
	virtual void RegisterComponentWithInterface_Implementation(UActorComponent* Component) override;

	/** Unregister a component from its interface */
	virtual void UnregisterComponentWithInterface_Implementation(UActorComponent* Component) override;

	/** Notifies components about registration state changes */
	bool NotifyComponentRegisterChanged(class UActorComponent* Component, bool Registering);

	void AddInputMapping(EInputSettingType SettingsType, EInputConfiguration Configuration);

	EInputSettingType GetInputSettingForClass(UClass* Class) const;
private:
	/** Maps registered interface classes to their components */
	UPROPERTY(Transient)
	TMap<UClass*, UActorComponent*> RegisteredComponentsByClass;

	/** Names of mapping contexts currently added (for diagnostic log on UnPossess) */
	UPROPERTY(Transient)
	TArray<FString> ActiveMappingContextNames;

private:
#pragma region Camera Controls
	/** Handles camera look input */
	void Look(const FInputActionValue& Value);

	/** Handles toggling camera view modes */
	void ToggleCamera(const FInputActionValue& Value);

	/** Handles zooming the camera in or out */
	void ZoomCamera(const FInputActionValue& Value);
#pragma endregion

#pragma region Turret Controls
	void Rotate(const FInputActionValue& Value);
#pragma endregion

#pragma region Engine Controls
	void StartEngineAction();
	void StopEngineAction();
	void StartSecondEngineAction();
	void StopSecondEngineAction();
#pragma endregion Engine Controls

#pragma region Systems Controls
	void ToggleSASAction();
#pragma endregion Systems Controls

#pragma region Humanoid Controls
	/** Handles jump input (start) */
	void Jump();

	/** Handles jump input (stop) */
	void StopJumping();

	/** Handles run input (start) */
	void Run();

	/** Handles run input (stop) */
	void StopRunning();

	/** Handles movement input */
	void Move(const FInputActionValue& Value);

	/** Handles interaction input (start) */
	void Interact();

	/** Handles interaction input (stop) */
	void StopInteracting();
#pragma endregion

#pragma region Vehicle Controls
	/** Handles steering input */
	void Steering(const FInputActionValue& Value);

	/** Handles steering end event */
	void SteeringCompleted(const FInputActionValue& Value);

	/** Handles throttle input */
	void Throttle(const FInputActionValue& Value);

	/** Handles brake input */
	void Brake(const FInputActionValue& Value);

	/** Handles brake (start) input */
	void StartBrake(const FInputActionValue& Value);

	/** Handles brake (stop) input */
	void StopBrake(const FInputActionValue& Value);

	/** Handles handbrake (start) input */
	void StartHandbrake(const FInputActionValue& Value);

	/** Handles handbrake (stop) input */
	void StopHandbrake(const FInputActionValue& Value);

	/** Handles vehicle reset input */
	void ResetVehicle(const FInputActionValue& Value);
#pragma endregion

#pragma region Helicopter Controls
	/** Handles cyclic input */
	void Cyclic(const FInputActionValue& Value);
	void CyclicCompleted(const FInputActionValue& Value);
	void CyclicKeyWStart();
	void CyclicKeyWStop();
	void CyclicKeyAStart();
	void CyclicKeyAStop();
	void CyclicKeySStart();
	void CyclicKeySStop();
	void CyclicKeyDStart();
	void CyclicKeyDStop();
	/** Handles collective input */
	void Collective(const FInputActionValue& Value);
	void CollectiveKeyStart();
	void CollectiveKeyStop();
	/** Handles pedals input */
	void Pedals(const FInputActionValue& Value);
	void PedalsKeyLeftStart();
	void PedalsKeyLeftStop();
	void PedalsKeyRightStart();
	void PedalsKeyRightStop();
#pragma endregion

	// Implementation of IPlayerControllerInterface
	virtual EViewMode GetCurrentViewMode_Implementation() const override { return ViewMode; }

	TArray<class UClass*> GetComponentInterfaceClass(UActorComponent* Component) const;

	bool CanInteractWithComponent(class UActorComponent* Component) const;

public:
	//~ Lyra-style functionality (uses Elemental interfaces only; no Core/GameFeature dependency)

	/** Gets the HUD (game may use a specific HUD class; this returns the engine base). */
	UFUNCTION(BlueprintCallable, Category = "WarCollection|PlayerController")
	class AHUD* GetWarCollectionHUD() const;

	/** Gets the ability system component from the player state (via IAbilitySystemInterface). */
	UFUNCTION(BlueprintCallable, Category = "WarCollection|PlayerController")
	class UAbilitySystemComponent* GetAbilitySystemComponent() const;

	/** Sets auto running state */
	UFUNCTION(BlueprintCallable, Category = "WarCollection|PlayerController")
	void SetIsAutoRunning(const bool bEnabled);

	/** Gets auto running state (from ability system when available). */
	UFUNCTION(BlueprintCallable, Category = "WarCollection|PlayerController")
	bool GetIsAutoRunning() const;

	// Run a cheat command on the server.
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCheat(const FString& Msg);

	// Run a cheat command on the server for all players.
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCheatAll(const FString& Msg);

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of AActor interface

	//~AController interface
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

	//~APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void SetPlayer(UPlayer* InPlayer) override;
	virtual void AddCheats(bool bForce) override;
	virtual void UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId) override;
	virtual void UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& OutHiddenComponents) override;
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PlayerTick(float DeltaTime) override;
	//~End of APlayerController interface

	//~ICameraAssistInterface interface
	virtual void OnCameraPenetratingTarget() override;
	//~End of ICameraAssistInterface interface

	//~ITeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ITeamAgentInterface interface

protected:
	// Called when the player state is set or cleared
	virtual void OnPlayerStateChanged();

	// Called when settings change (Settings may implement ISharedSettingsForceFeedbackInterface)
	UFUNCTION()
	void OnSettingsChanged(UObject* Settings);

	// Try to record a client replay
	UFUNCTION(BlueprintCallable, Category = "WarCollection|PlayerController")
	bool TryToRecordClientReplay();

	// Check if we should record a replay
	virtual bool ShouldRecordClientReplay();

private:
	void BroadcastOnPlayerStateChanged();

	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStartAutoRun"))
	void K2_OnStartAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnEndAutoRun"))
	void K2_OnEndAutoRun();

private:
	/** Auto running state */
	bool bIsAutoRunning = false;

	/** Called when auto run starts */
	void OnStartAutoRun();

	/** Called when auto run ends */
	void OnEndAutoRun();

private:
	/** Last seen player state for team tracking */
	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;

	/** Team change delegate */
	UPROPERTY()
	FOnTeamIndexChangedDelegate OnTeamChangedDelegate;

	/** Force feedback enabled flag */
	bool bForceFeedbackEnabled = true;

	/** Force feedback values */
	FForceFeedbackValues ForceFeedbackValues;

	/** Flag to hide view target pawn next frame (for camera penetration) */
	bool bHideViewTargetPawnNextFrame = false;
};