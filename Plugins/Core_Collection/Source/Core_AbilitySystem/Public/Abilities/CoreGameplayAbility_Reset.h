// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "Abilities/CoreGameplayAbility.h"

#include "CoreGameplayAbility_Reset.generated.h"

class AActor;
class UObject;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;

/**
 * UCoreGameplayAbility_Reset
 *
 *	Gameplay ability used for handling quickly resetting the player back to initial spawn state.
 *	Ability is activated automatically via the "GameplayEvent.RequestReset" ability trigger tag (server only).
 *	Adapted from ULyraGameplayAbility_Reset.
 */
UCLASS()
class CORE_ABILITYSYSTEM_API UCoreGameplayAbility_Reset : public UCoreGameplayAbility
{
	GENERATED_BODY()

public:
	UCoreGameplayAbility_Reset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};

USTRUCT(BlueprintType)
struct CORE_ABILITYSYSTEM_API FCorePlayerResetMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwnerPlayerState = nullptr;
};
