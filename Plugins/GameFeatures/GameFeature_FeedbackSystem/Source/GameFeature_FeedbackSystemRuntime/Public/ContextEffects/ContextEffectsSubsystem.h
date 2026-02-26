// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"

#include "ContextEffectsSubsystem.generated.h"

enum EPhysicalSurface : int;

class AActor;
class UAudioComponent;
class UContextEffectsLibrary;
class UNiagaraComponent;
class USceneComponent;
struct FFrame;
struct FGameplayTag;
struct FGameplayTagContainer;

/**
 * Settings for Context Effects
 */
UCLASS(MinimalAPI, config = Game, defaultconfig, meta = (DisplayName = "Context Effects"))
class UContextEffectsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere)
	TMap<TEnumAsByte<EPhysicalSurface>, FGameplayTag> SurfaceTypeToContextMap;
};

/**
 * Set of Context Effects Libraries for an Actor
 */
UCLASS(MinimalAPI)
class UContextEffectsSet : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	TSet<TObjectPtr<UContextEffectsLibrary>> ContextEffectsLibraries;
};

/**
 * Subsystem for managing Context Effects
 */
UCLASS(MinimalAPI)
class UContextEffectsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Spawn context effects at the specified location */
	UFUNCTION(BlueprintCallable, Category = "Feedback|Context Effects")
	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API void SpawnContextEffects(
		const AActor* SpawningActor,
		USceneComponent* AttachToComponent,
		const FName AttachPoint,
		const FVector LocationOffset,
		const FRotator RotationOffset,
		FGameplayTag Effect,
		FGameplayTagContainer Contexts,
		TArray<UAudioComponent*>& AudioOut,
		TArray<UNiagaraComponent*>& NiagaraOut,
		FVector VFXScale = FVector(1),
		float AudioVolume = 1,
		float AudioPitch = 1);

	/** Get context tag from physical surface type */
	UFUNCTION(BlueprintCallable, Category = "Feedback|Context Effects")
	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API bool GetContextFromSurfaceType(TEnumAsByte<EPhysicalSurface> PhysicalSurface, FGameplayTag& Context);

	/** Load and add context effects libraries for an actor */
	UFUNCTION(BlueprintCallable, Category = "Feedback|Context Effects")
	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API void LoadAndAddContextEffectsLibraries(AActor* OwningActor, TSet<TSoftObjectPtr<UContextEffectsLibrary>> ContextEffectsLibraries);

	/** Unload and remove context effects libraries for an actor */
	UFUNCTION(BlueprintCallable, Category = "Feedback|Context Effects")
	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API void UnloadAndRemoveContextEffectsLibraries(AActor* OwningActor);

private:

	UPROPERTY(Transient)
	TMap<TObjectPtr<AActor>, TObjectPtr<UContextEffectsSet>> ActiveActorEffectsMap;
};
