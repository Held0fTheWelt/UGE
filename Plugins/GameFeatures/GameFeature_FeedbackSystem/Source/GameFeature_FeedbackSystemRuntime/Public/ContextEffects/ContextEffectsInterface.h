// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/HitResult.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"

#include "ContextEffectsInterface.generated.h"

class UAnimSequenceBase;
class UObject;
class USceneComponent;
struct FFrame;

/**
 * Match type for context effects
 */
UENUM()
enum EEffectsContextMatchType : int
{
	ExactMatch,
	BestMatch
};

/**
 * Interface for context effects. Used for framework communication via Elemental_Collection.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UContextEffectsInterface : public UInterface
{
	GENERATED_BODY()
};

class GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API IContextEffectsInterface : public IInterface
{
	GENERATED_BODY()

public:

	/** Called when an animation motion effect should be triggered */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Feedback|Context Effects")
	void AnimMotionEffect(
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
		float AudioPitch = 1);
};
