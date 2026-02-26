// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "ContextEffects/ContextEffectsInterface.h"

#include "ContextEffectComponent.generated.h"

namespace EEndPlayReason { enum Type : int; }

class UAnimSequenceBase;
class UAudioComponent;
class UContextEffectsLibrary;
class UNiagaraComponent;
class UObject;
class USceneComponent;
struct FFrame;
struct FHitResult;

/**
 * Component that provides context effects (VFX, audio) for animation motion and hits.
 * Implements IContextEffectsInterface for framework communication.
 */
UCLASS(MinimalAPI, ClassGroup=(Custom), hidecategories = (Variable, Tags, ComponentTick, ComponentReplication, Activation, Cooking, AssetUserData, Collision), CollapseCategories, meta=(BlueprintSpawnableComponent))
class UContextEffectComponent : public UActorComponent, public IContextEffectsInterface
{
	GENERATED_BODY()

public:

	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API UContextEffectComponent();

protected:

	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API virtual void BeginPlay() override;
	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	UFUNCTION(BlueprintCallable)
	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API virtual void AnimMotionEffect_Implementation(
		const FName Bone,
		const FGameplayTag MotionEffect,
		USceneComponent* StaticMeshComponent,
		const FVector LocationOffset,
		const FRotator RotationOffset,
		const UAnimSequenceBase* AnimationSequence,
		const bool bHitSuccess,
		const FHitResult HitResult,
		FGameplayTagContainer Contexts,
		FVector VFXScale = FVector(1),
		float AudioVolume = 1,
		float AudioPitch = 1) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bConvertPhysicalSurfaceToContext = true;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer DefaultEffectContexts;

	UPROPERTY(EditAnywhere)
	TSet<TSoftObjectPtr<UContextEffectsLibrary>> DefaultContextEffectsLibraries;

	UFUNCTION(BlueprintCallable)
	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API void UpdateEffectContexts(FGameplayTagContainer NewEffectContexts);

	UFUNCTION(BlueprintCallable)
	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API void UpdateLibraries(TSet<TSoftObjectPtr<UContextEffectsLibrary>> NewContextEffectsLibraries);

private:

	UPROPERTY(Transient)
	FGameplayTagContainer CurrentContexts;

	UPROPERTY(Transient)
	TSet<TSoftObjectPtr<UContextEffectsLibrary>> CurrentContextEffectsLibraries;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UAudioComponent>> ActiveAudioComponents;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> ActiveNiagaraComponents;
};
