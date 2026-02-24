// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Abilities/CoreGameplayAbility_Death.h"
#include "Abilities/CoreGameplayAbility.h"
#include "Components/CoreAbilitySystemComponent.h"
#include "Elemental_Interfaces/Public/Interfaces/DeathExecutionInterface.h"
#include "GameplayTags_Core.h"
#include "GameFramework/Actor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreGameplayAbility_Death)

static UObject* FindDeathExecutionObject(AActor* Actor)
{
	if (!Actor) return nullptr;
	for (UActorComponent* Comp : Actor->GetComponents())
	{
		if (Comp && Comp->GetClass()->ImplementsInterface(UDeathExecutionInterface::StaticClass()))
		{
			return Comp;
		}
	}
	return (Actor->GetClass()->ImplementsInterface(UDeathExecutionInterface::StaticClass()) ? Actor : nullptr);
}

UCoreGameplayAbility_Death::UCoreGameplayAbility_Death(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	bAutoStartDeath = true;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = GameplayTags_Core::GameplayEvent_Death;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UCoreGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo);

	UCoreAbilitySystemComponent* CoreASC = CastChecked<UCoreAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(GameplayTags_Core::Ability_Behavior_SurvivesDeath);

	// Cancel all abilities and block others from starting.
	CoreASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);

	SetCanBeCanceled(false);

	if (!ChangeActivationGroup(EAbilityActivationGroup::Exclusive_Blocking))
	{
		UE_LOG(LogTemp, Error, TEXT("UCoreGameplayAbility_Death::ActivateAbility: Ability [%s] failed to change activation group to blocking."), *GetName());
	}

	if (bAutoStartDeath)
	{
		StartDeath();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCoreGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	check(ActorInfo);

	// Always try to finish the death when the ability ends in case the ability doesn't.
	// This won't do anything if the death hasn't been started.
	FinishDeath();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCoreGameplayAbility_Death::StartDeath()
{
	if (UObject* Obj = FindDeathExecutionObject(GetAvatarActorFromActorInfo()))
	{
		if (IDeathExecutionInterface* DeathExec = Cast<IDeathExecutionInterface>(Obj))
		{
			if (DeathExec->GetDeathState() == EDeathExecutionState::NotDead)
			{
				DeathExec->StartDeath();
			}
		}
	}
}

void UCoreGameplayAbility_Death::FinishDeath()
{
	if (UObject* Obj = FindDeathExecutionObject(GetAvatarActorFromActorInfo()))
	{
		if (IDeathExecutionInterface* DeathExec = Cast<IDeathExecutionInterface>(Obj))
		{
			if (DeathExec->GetDeathState() == EDeathExecutionState::DeathStarted)
			{
				DeathExec->FinishDeath();
			}
		}
	}
}
