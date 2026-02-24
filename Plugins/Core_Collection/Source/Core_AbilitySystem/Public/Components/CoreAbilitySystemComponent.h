// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Enums/AbilityActivationGroup.h"
#include "Enums/AbilityActivationPolicy.h"
#include "Structs/AbilityMontageFailureMessage.h"
#include "NativeGameplayTags.h"
#include "Interfaces/AbilitySystemInputInterface.h"
#include "Interfaces/AbilitySystemTagRelationshipReceiverInterface.h"
#include "CoreAbilitySystemComponent.generated.h"

struct FFrame;
struct FGameplayAbilityTargetDataHandle;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityInputBlocked);

/**
 * UCoreAbilitySystemComponent
 *
 *	Base ability system component class used by this project.
 */
UCLASS()
class CORE_ABILITYSYSTEM_API UCoreAbilitySystemComponent : public UAbilitySystemComponent, public IAbilitySystemInputInterface, public IAbilitySystemTagRelationshipReceiverInterface
{
	GENERATED_BODY()
	
public:

	UCoreAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	typedef TFunctionRef<bool(const class UCoreGameplayAbility* LyraAbility, FGameplayAbilitySpecHandle Handle)> TShouldCancelAbilityFunc;
	void CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility);

	void CancelInputActivatedAbilities(bool bReplicateCancelAbility);

	//~IAbilitySystemInputInterface (same signature as interface for HeroSystem/Controllers)
	void AbilityInputTagPressed(FGameplayTag InputTag) override;
	void AbilityInputTagReleased(FGameplayTag InputTag) override;

	void ClearAbilityInput();

	bool IsActivationGroupBlocked(EAbilityActivationGroup Group) const;
	void AddAbilityToActivationGroup(EAbilityActivationGroup Group, class UCoreGameplayAbility* LyraAbility);
	void RemoveAbilityFromActivationGroup(EAbilityActivationGroup Group, class UCoreGameplayAbility* LyraAbility);
	void CancelActivationGroupAbilities(EAbilityActivationGroup Group, class UCoreGameplayAbility* IgnoreLyraAbility, bool bReplicateCancelAbility);

	// Uses a gameplay effect to add the specified dynamic granted tag.
	void AddDynamicTagGameplayEffect(const FGameplayTag& Tag);

	// Removes all active instances of the gameplay effect that was used to add the specified dynamic granted tag.
	void RemoveDynamicTagGameplayEffect(const FGameplayTag& Tag);

	/** Gets the ability target data associated with the given ability handle and activation info */
	void GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle);

	/** Sets the current tag relationship mapping, if null it will clear it out */
	void SetTagRelationshipMapping(class UAbilityTagRelationshipMapping* NewMapping);

	//~IAbilitySystemTagRelationshipReceiverInterface (for GameFeature_PawnExtension via interface only)
	void SetTagRelationshipMapping(UObject* Mapping) override;

	/** Looks at ability tags and gathers additional required and blocking tags */
	void GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const;

	//~IAbilitySystemInputInterface (for Controllers plugin; no Core dependency there)
	void TryActivateAbilitiesOnSpawn() override;

	int32 GetTagCount(FGameplayTag Tag) const override;
	void SetLooseGameplayTagCount(FGameplayTag Tag, int32 Count) override;
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused) override;

protected:

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, class UGameplayAbility* Ability) override;
	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, class UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, class UGameplayAbility* Ability, bool bWasCancelled) override;
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, class UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) override;
	virtual void HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags, class UGameplayAbility* RequestingAbility, bool bCanBeCanceled) override;

	/** Notify client that an ability failed to activate */
	UFUNCTION(Client, Unreliable)
	void ClientNotifyAbilityFailed(const class UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);

	void HandleAbilityFailed(const class UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);
protected:

	// If set, this table is used to look up tag relationships for activate and cancel
	UPROPERTY()
	TObjectPtr<class UAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	// Number of abilities running in each activation group.
	int32 ActivationGroupCounts[(uint8)EAbilityActivationGroup::MAX];
};
