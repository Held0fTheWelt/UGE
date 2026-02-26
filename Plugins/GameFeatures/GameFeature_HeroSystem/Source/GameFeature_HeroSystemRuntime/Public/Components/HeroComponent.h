// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "GameplayAbilitySpecHandle.h"

#include "HeroComponent.generated.h"

namespace EEndPlayReason { enum Type : int; }
struct FGameplayTag;
struct FInputActionValue;

class UGameFrameworkComponentManager;
class UInputComponent;
class UObject;
struct FActorInitStateChangedParams;
struct FFrame;

/**
 * Component that sets up input and camera handling for player controlled pawns (or bots that simulate players).
 * Depends on a PawnExtensionComponent (via IPawnExtensionComponentInterface) to coordinate initialization.
 * Uses only Elemental_Collection interfaces and UElementalInitStateTagSubsystem; no direct Core or GameFeature_PawnExtension dependency.
 */
UCLASS(MinimalAPI, Blueprintable, Meta=(BlueprintSpawnableComponent))
class UHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:

	GAMEFEATURE_HEROSYSTEMRUNTIME_API UHeroComponent(const FObjectInitializer& ObjectInitializer);

	/** Returns the hero component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Hero")
	static GAMEFEATURE_HEROSYSTEMRUNTIME_API UHeroComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UHeroComponent>() : nullptr); }

	/** Overrides the camera from an active gameplay ability */
	GAMEFEATURE_HEROSYSTEMRUNTIME_API void SetAbilityCameraMode(TSubclassOf<UObject> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle);

	/** Clears the camera override if it is set */
	GAMEFEATURE_HEROSYSTEMRUNTIME_API void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle);

	/** True if this is controlled by a real player and has progressed far enough in initialization where additional input bindings can be added */
	GAMEFEATURE_HEROSYSTEMRUNTIME_API bool IsReadyToBindInputs() const;

	/** The name of the extension event sent via UGameFrameworkComponentManager when ability inputs are ready to bind */
	static GAMEFEATURE_HEROSYSTEMRUNTIME_API const FName NAME_BindInputsNow;

	/** The name of this component-implemented feature */
	static GAMEFEATURE_HEROSYSTEMRUNTIME_API const FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	GAMEFEATURE_HEROSYSTEMRUNTIME_API virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	GAMEFEATURE_HEROSYSTEMRUNTIME_API virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	GAMEFEATURE_HEROSYSTEMRUNTIME_API virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	GAMEFEATURE_HEROSYSTEMRUNTIME_API virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface

protected:

	GAMEFEATURE_HEROSYSTEMRUNTIME_API virtual void OnRegister() override;
	GAMEFEATURE_HEROSYSTEMRUNTIME_API virtual void BeginPlay() override;
	GAMEFEATURE_HEROSYSTEMRUNTIME_API virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Initialize player input (minimal version - will be extended in later phases) */
	GAMEFEATURE_HEROSYSTEMRUNTIME_API virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	/** Handle ability input tag pressed (forwards to ASC via IAbilitySystemInputInterface) */
	GAMEFEATURE_HEROSYSTEMRUNTIME_API void Input_AbilityInputTagPressed(FGameplayTag InputTag);

	/** Handle ability input tag released */
	GAMEFEATURE_HEROSYSTEMRUNTIME_API void Input_AbilityInputTagReleased(FGameplayTag InputTag);

protected:

	/** Camera mode set by an ability. */
	UPROPERTY()
	TSubclassOf<UObject> AbilityCameraMode;

	/** Spec handle for the last ability to set a camera mode. */
	FGameplayAbilitySpecHandle AbilityCameraModeOwningSpecHandle;

	/** True when player input bindings have been applied; will never be true for non-players */
	bool bReadyToBindInputs;
};
