// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "Structs/InteractionOption.h"
#include "GameplayAbility_Interact.generated.h"

class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;

/** Gameplay ability used for interacting with objects. Registers GrantNearbyInteraction and WaitForInteractableTargets; TriggerInteraction runs the first available option. */
UCLASS(Abstract)
class GAMEFEATURE_INTERACTIONSYSTEMRUNTIME_API UGameplayAbility_Interact : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Applies the first available interaction option (call from input or UI). */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TriggerInteraction();

	/** Called when the line-trace task updates options; stores them for TriggerInteraction. */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UpdateInteractions(const TArray<FInteractionOption>& InteractiveOptions);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Currently available options from the last trace (first = preferred target). */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	TArray<FInteractionOption> CurrentOptions;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionScanRange = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionScanRate = 0.1f;
};
