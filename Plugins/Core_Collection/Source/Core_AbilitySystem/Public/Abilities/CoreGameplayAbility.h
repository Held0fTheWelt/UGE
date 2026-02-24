// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Abilities/GameplayAbility.h"
#include "Enums/AbilityActivationPolicy.h"
#include "Enums/AbilityActivationGroup.h"
#include "Structs/AbilityMontageFailureMessage.h"

#include "CoreGameplayAbility.generated.h"

struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpec;
struct FGameplayAbilitySpecHandle;

class AActor;
class AController;
class APlayerController;
class FText;
class UAnimMontage;
class UCoreAbilitySystemComponent;
class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEffectSpec;
struct FGameplayEventData;

// Forward declarations (Core_AbilitySystem does not depend on Core_Classes; cast to ACoreCharacter/ACombinedBundleCharacter in callers)
class ACharacter;

/**
 * UCoreGameplayAbility
 *
 *	The base gameplay ability class used by this project.
 *	Adapted from ULyraGameplayAbility.
 */
UCLASS(MinimalAPI, Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class UCoreGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class UCoreAbilitySystemComponent;

public:

	CORE_ABILITYSYSTEM_API UCoreGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Core|Ability")
	CORE_ABILITYSYSTEM_API UCoreAbilitySystemComponent* GetCoreAbilitySystemComponentFromActorInfo() const;

	/** Returns the player controller from actor info; cast to your project's controller type (e.g. UnifiedPlayerController) if needed. */
	UFUNCTION(BlueprintCallable, Category = "Core|Ability")
	CORE_ABILITYSYSTEM_API APlayerController* GetPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Core|Ability")
	CORE_ABILITYSYSTEM_API AController* GetControllerFromActorInfo() const;

	/** Returns the avatar as ACharacter; cast to ACoreCharacter or ACombinedBundleCharacter in Core_Classes if needed. */
	UFUNCTION(BlueprintCallable, Category = "Core|Ability")
	ACharacter* GetCoreCharacterFromActorInfo() const;

	/** Returns the avatar as ACharacter; cast to ACombinedBundleCharacter in Core_Classes if needed. */
	UFUNCTION(BlueprintCallable, Category = "Core|Ability")
	ACharacter* GetCombinedBundleCharacterFromActorInfo() const;

	EAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	EAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }

	CORE_ABILITYSYSTEM_API void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	// Returns true if the requested activation group is a valid transition.
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Core|Ability", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	CORE_ABILITYSYSTEM_API bool CanChangeActivationGroup(EAbilityActivationGroup NewGroup) const;

	// Tries to change the activation group.  Returns true if it successfully changed.
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Core|Ability", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	CORE_ABILITYSYSTEM_API bool ChangeActivationGroup(EAbilityActivationGroup NewGroup);

	// Sets the ability's camera mode.
	// Note: This requires camera system integration - currently placeholder
	UFUNCTION(BlueprintCallable, Category = "Core|Ability")
	CORE_ABILITYSYSTEM_API void SetCameraMode(TSubclassOf<UObject> CameraMode);

	// Clears the ability's camera mode.  Automatically called if needed when the ability ends.
	UFUNCTION(BlueprintCallable, Category = "Core|Ability")
	CORE_ABILITYSYSTEM_API void ClearCameraMode();

	void OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const
	{
		NativeOnAbilityFailedToActivate(FailedReason);
		ScriptOnAbilityFailedToActivate(FailedReason);
	}

protected:

	// Called when the ability fails to activate
	CORE_ABILITYSYSTEM_API virtual void NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	// Called when the ability fails to activate
	UFUNCTION(BlueprintImplementableEvent)
	CORE_ABILITYSYSTEM_API void ScriptOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	//~UGameplayAbility interface
	CORE_ABILITYSYSTEM_API virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	CORE_ABILITYSYSTEM_API virtual void SetCanBeCanceled(bool bCanBeCanceled) override;
	CORE_ABILITYSYSTEM_API virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	CORE_ABILITYSYSTEM_API virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	CORE_ABILITYSYSTEM_API virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	CORE_ABILITYSYSTEM_API virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	CORE_ABILITYSYSTEM_API virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	CORE_ABILITYSYSTEM_API virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	CORE_ABILITYSYSTEM_API virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
	CORE_ABILITYSYSTEM_API virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const override;
	CORE_ABILITYSYSTEM_API virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//~End of UGameplayAbility interface

	CORE_ABILITYSYSTEM_API virtual void OnPawnAvatarSet();

	CORE_ABILITYSYSTEM_API virtual void GetAbilitySource(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, float& OutSourceLevel, const class ICoreAbilitySourceInterface*& OutAbilitySource, AActor*& OutEffectCauser) const;

	/** Called when this ability is granted to the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
	CORE_ABILITYSYSTEM_API void K2_OnAbilityAdded();

	/** Called when this ability is removed from the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityRemoved")
	CORE_ABILITYSYSTEM_API void K2_OnAbilityRemoved();

	/** Called when the ability system is initialized with a pawn avatar. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnPawnAvatarSet")
	CORE_ABILITYSYSTEM_API void K2_OnPawnAvatarSet();

protected:

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|Ability Activation")
	EAbilityActivationPolicy ActivationPolicy;

	// Defines the relationship between this ability activating and other abilities activating.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|Ability Activation")
	EAbilityActivationGroup ActivationGroup;

	// Additional costs that must be paid to activate this ability
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Costs)
	TArray<TObjectPtr<class UCoreAbilityCost>> AdditionalCosts;

	// Map of failure tags to simple error messages
	UPROPERTY(EditDefaultsOnly, Category = "Advanced")
	TMap<FGameplayTag, FText> FailureTagToUserFacingMessages;

	// Map of failure tags to anim montages that should be played with them
	UPROPERTY(EditDefaultsOnly, Category = "Advanced")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> FailureTagToAnimMontage;

	// If true, extra information should be logged when this ability is canceled. This is temporary, used for tracking a bug.
	UPROPERTY(EditDefaultsOnly, Category = "Advanced")
	bool bLogCancelation;

	// Current camera mode set by the ability.
	TSubclassOf<UObject> ActiveCameraMode;
};
