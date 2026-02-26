// Copyright Epic Games, Inc. All Rights Reserved.

#include "Abilities/GameplayAbility_Interact.h"
#include "Interfaces/IInteractableTarget.h"
#include "InteractionStatics.h"
#include "Structs/InteractionQuery.h"
#include "Structs/InteractionOption.h"
#include "Tasks/AbilityTask_GrantNearbyInteraction.h"
#include "Tasks/AbilityTask_WaitForInteractableTargets_SingleLineTrace.h"
#include "AbilitySystemComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/Controller.h"
#include "NativeGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayAbility_Interact)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Ability_Interaction_Activate, "Ability.Interaction.Activate");

UGameplayAbility_Interact::UGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AbilitySystem || !AvatarActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Grant abilities from nearby interactables (Authority only, so clients get them via replication)
	if (AbilitySystem->GetOwnerRole() == ROLE_Authority)
	{
		UAbilityTask_GrantNearbyInteraction* GrantTask = UAbilityTask_GrantNearbyInteraction::GrantAbilitiesForNearbyInteractors(this, InteractionScanRange, InteractionScanRate);
		if (GrantTask)
		{
			GrantTask->ReadyForActivation();
		}
	}

	// Line-trace for current target and feed options into CurrentOptions
	FInteractionQuery InteractionQuery;
	InteractionQuery.RequestingAvatar = AvatarActor;
	InteractionQuery.RequestingController = Cast<AController>(AvatarActor->GetOwner());

	FGameplayAbilityTargetingLocationInfo StartLocation;
	StartLocation.SourceActor = AvatarActor;
	StartLocation.SourceComponent = Cast<UMeshComponent>(AvatarActor->GetRootComponent());

	FCollisionProfileName TraceProfile(UCollisionProfile::Pawn_ProfileName);

	UAbilityTask_WaitForInteractableTargets_SingleLineTrace* WaitTask = UAbilityTask_WaitForInteractableTargets_SingleLineTrace::WaitForInteractableTargets_SingleLineTrace(
		this, InteractionQuery, TraceProfile, StartLocation, InteractionScanRange, InteractionScanRate, false);
	if (WaitTask)
	{
		WaitTask->InteractableObjectsChanged.AddDynamic(this, &UGameplayAbility_Interact::UpdateInteractions);
		WaitTask->ReadyForActivation();
	}

	// Ability stays active; tasks run until ability is ended
}

void UGameplayAbility_Interact::UpdateInteractions(const TArray<FInteractionOption>& InteractiveOptions)
{
	CurrentOptions = InteractiveOptions;
}

void UGameplayAbility_Interact::TriggerInteraction()
{
	if (CurrentOptions.Num() == 0)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystem)
	{
		return;
	}

	const FInteractionOption& InteractionOption = CurrentOptions[0];
	AActor* Instigator = GetAvatarActorFromActorInfo();
	AActor* InteractableTargetActor = UInteractionStatics::GetActorFromInteractableTarget(InteractionOption.InteractableTarget);
	if (!InteractableTargetActor || !InteractionOption.TargetAbilitySystem || !InteractionOption.TargetInteractionAbilityHandle.IsValid())
	{
		return;
	}

	FGameplayEventData Payload;
	Payload.EventTag = TAG_Ability_Interaction_Activate.GetTag();
	Payload.Instigator = Instigator;
	Payload.Target = InteractableTargetActor;

	InteractionOption.InteractableTarget->CustomizeInteractionEventData(TAG_Ability_Interaction_Activate.GetTag(), Payload);

	AActor* TargetActor = const_cast<AActor*>(ToRawPtr(Payload.Target));

	FGameplayAbilityActorInfo ActorInfo;
	ActorInfo.InitFromActor(InteractableTargetActor, TargetActor, InteractionOption.TargetAbilitySystem);

	InteractionOption.TargetAbilitySystem->TriggerAbilityFromGameplayEvent(
		InteractionOption.TargetInteractionAbilityHandle,
		&ActorInfo,
		TAG_Ability_Interaction_Activate.GetTag(),
		&Payload,
		*InteractionOption.TargetAbilitySystem
	);
}
