// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "Abilities/CoreGameplayAbility.h"

#include "CoreGameplayAbility_Jump.generated.h"

class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

/**
 * UCoreGameplayAbility_Jump
 *
 *	Gameplay ability used for character jumping.
 *	Adapted from ULyraGameplayAbility_Jump.
 */
UCLASS(Abstract)
class CORE_ABILITYSYSTEM_API UCoreGameplayAbility_Jump : public UCoreGameplayAbility
{
	GENERATED_BODY()

public:

	UCoreGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Core|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Core|Ability")
	void CharacterJumpStop();
};
