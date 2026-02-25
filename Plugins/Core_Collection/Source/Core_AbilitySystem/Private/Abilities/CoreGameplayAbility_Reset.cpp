// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Abilities/CoreGameplayAbility_Reset.h"
#include "Components/CoreAbilitySystemComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTags_Core.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreGameplayAbility_Reset)

UCoreGameplayAbility_Reset::UCoreGameplayAbility_Reset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = GameplayTags_Core::GameplayEvent_RequestReset;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UCoreGameplayAbility_Reset::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo);

	UCoreAbilitySystemComponent* CoreASC = CastChecked<UCoreAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(GameplayTags_Core::Ability_Behavior_SurvivesDeath);

	// Cancel all abilities and block others from starting.
	CoreASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);

	SetCanBeCanceled(false);

	// Execute the reset from the character
	// Note: Reset functionality needs to be implemented in Character or via GameFeature
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor)
	{
		// TODO: Implement Reset functionality in Character or via GameFeature
		// For now, we'll just end the ability
		// if (ACoreCharacter* CoreChar = Cast<ACoreCharacter>(AvatarActor))
		// {
		// 	CoreChar->Reset();
		// }
	}

	// Let others know a reset has occurred
	FCorePlayerResetMessage Message;
	Message.OwnerPlayerState = ActorInfo->OwnerActor.Get();
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(GameplayTags_Core::GameplayEvent_Reset, Message);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bReplicateEndAbility = true;
	const bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);
}
