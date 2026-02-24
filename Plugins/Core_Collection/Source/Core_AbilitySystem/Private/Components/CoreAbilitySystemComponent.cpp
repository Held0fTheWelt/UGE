// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CoreAbilitySystemComponent.h"

#include "Abilities/CoreGameplayAbility.h"
#include "DataAssets/AbilityTagRelationshipMapping.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Subsystems/GlobalAbilitySubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreAbilitySystemComponent)

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");

UCoreAbilitySystemComponent::UCoreAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();

	FMemory::Memset(ActivationGroupCounts, 0, sizeof(ActivationGroupCounts));
}

void UCoreAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UGlobalAbilitySubsystem* GlobalAbilitySystem = UWorld::GetSubsystem<UGlobalAbilitySubsystem>(GetWorld()))
	{
		GlobalAbilitySystem->UnregisterASC(this);
	}

	Super::EndPlay(EndPlayReason);
}

void UCoreAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

		if (bHasNewPawnAvatar)
		{
			// Notify all abilities that a new pawn avatar has been set
			for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
			{
				PRAGMA_DISABLE_DEPRECATION_WARNINGS
					ensureMsgf(AbilitySpec.Ability && AbilitySpec.Ability->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced, TEXT("InitAbilityActorInfo: All Abilities should be Instanced (NonInstanced is being deprecated due to usability issues)."));
				PRAGMA_ENABLE_DEPRECATION_WARNINGS

					TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
				for (UGameplayAbility* AbilityInstance : Instances)
				{
					UCoreGameplayAbility* CoreAbilityInstance = Cast<UCoreGameplayAbility>(AbilityInstance);
					if (CoreAbilityInstance)
					{
						// Ability instances may be missing for replays
						CoreAbilityInstance->OnPawnAvatarSet();
					}
				}
			}

		// Register with the global system once we actually have a pawn avatar. We wait until this time since some globally-applied effects may require an avatar.
		if (UGlobalAbilitySubsystem* GlobalAbilitySystem = UWorld::GetSubsystem<UGlobalAbilitySubsystem>(GetWorld()))
		{
			GlobalAbilitySystem->RegisterASC(this);
		}

		//if (ULyraAnimInstance* LyraAnimInst = Cast<ULyraAnimInstance>(ActorInfo->GetAnimInstance()))
		//{
		//	LyraAnimInst->InitializeWithAbilitySystem(this);
		//}

		TryActivateAbilitiesOnSpawn();
	}
}

void UCoreAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (const UCoreGameplayAbility* CoreAbilityCDO = Cast<UCoreGameplayAbility>(AbilitySpec.Ability))
		{
			CoreAbilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
		}
	}
}

void UCoreAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility)
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!AbilitySpec.IsActive())
		{
			continue;
		}

		UCoreGameplayAbility* LyraAbilityCDO = Cast<UCoreGameplayAbility>(AbilitySpec.Ability);
		if (!LyraAbilityCDO)
		{
			//UE_LOG(LogLyraAbilitySystem, Error, TEXT("CancelAbilitiesByFunc: Non-LyraGameplayAbility %s was Granted to ASC. Skipping."), *AbilitySpec.Ability.GetName());
			continue;
		}

		PRAGMA_DISABLE_DEPRECATION_WARNINGS
			ensureMsgf(AbilitySpec.Ability->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced, TEXT("CancelAbilitiesByFunc: All Abilities should be Instanced (NonInstanced is being deprecated due to usability issues)."));
		PRAGMA_ENABLE_DEPRECATION_WARNINGS

			// Cancel all the spawned instances.
			TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
		for (UGameplayAbility* AbilityInstance : Instances)
		{
			UCoreGameplayAbility* CoreAbilityInstance = CastChecked<UCoreGameplayAbility>(AbilityInstance);

			if (ShouldCancelFunc(CoreAbilityInstance, AbilitySpec.Handle))
			{
				if (CoreAbilityInstance->CanBeCanceled())
				{
					CoreAbilityInstance->CancelAbility(AbilitySpec.Handle, AbilityActorInfo.Get(), CoreAbilityInstance->GetCurrentActivationInfo(), bReplicateCancelAbility);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("CancelAbilitiesByFunc: Can't cancel ability [%s] because CanBeCanceled is false."), *CoreAbilityInstance->GetName());
				}
			}
		}
	}
}

void UCoreAbilitySystemComponent::CancelInputActivatedAbilities(bool bReplicateCancelAbility)
{
	auto ShouldCancelFunc = [this](const UCoreGameplayAbility* CoreAbility, FGameplayAbilitySpecHandle Handle)
		{
			const EAbilityActivationPolicy ActivationPolicy = CoreAbility->GetActivationPolicy();
			return ((ActivationPolicy == EAbilityActivationPolicy::OnInputTriggered) || (ActivationPolicy == EAbilityActivationPolicy::WhileInputActive));
		};

	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
}

void UCoreAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
			const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
		PRAGMA_ENABLE_DEPRECATION_WARNINGS

			// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, OriginalPredictionKey);
	}
}

void UCoreAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
			const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
		PRAGMA_ENABLE_DEPRECATION_WARNINGS

			// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, OriginalPredictionKey);
	}
}

void UCoreAbilitySystemComponent::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UCoreAbilitySystemComponent::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UCoreAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	//@TODO: See if we can use FScopedServerAbilityRPCBatcher ScopedRPCBatcher in some of these loops

	//
	// Process all abilities that activate when the input is held.
	//
		for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
		{
			if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
			{
				if (AbilitySpec->Ability && !AbilitySpec->IsActive())
				{
					const UCoreGameplayAbility* CoreAbilityCDO = Cast<UCoreGameplayAbility>(AbilitySpec->Ability);
					if (CoreAbilityCDO && CoreAbilityCDO->GetActivationPolicy() == EAbilityActivationPolicy::WhileInputActive)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}

	//
	// Process all abilities that had their input pressed this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UCoreGameplayAbility* CoreAbilityCDO = Cast<UCoreGameplayAbility>(AbilitySpec->Ability);

					if (CoreAbilityCDO && CoreAbilityCDO->GetActivationPolicy() == EAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UCoreAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

//~IAbilitySystemInputInterface (for Controllers plugin)
int32 UCoreAbilitySystemComponent::GetTagCount(FGameplayTag Tag) const
{
	return UAbilitySystemComponent::GetTagCount(Tag);
}

void UCoreAbilitySystemComponent::SetLooseGameplayTagCount(FGameplayTag Tag, int32 Count)
{
	UAbilitySystemComponent::SetLooseGameplayTagCount(Tag, Count);
}

void UCoreAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);

	if (UCoreGameplayAbility* CoreAbility = Cast<UCoreGameplayAbility>(Ability))
	{
		AddAbilityToActivationGroup(CoreAbility->GetActivationGroup(), CoreAbility);
	}
}

void UCoreAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	Super::NotifyAbilityFailed(Handle, Ability, FailureReason);

	if (APawn* Avatar = Cast<APawn>(GetAvatarActor()))
	{
		if (!Avatar->IsLocallyControlled() && Ability->IsSupportedForNetworking())
		{
			ClientNotifyAbilityFailed(Ability, FailureReason);
			return;
		}
	}

	HandleAbilityFailed(Ability, FailureReason);
}

void UCoreAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);

	if (UCoreGameplayAbility* CoreAbility = Cast<UCoreGameplayAbility>(Ability))
	{
		RemoveAbilityFromActivationGroup(CoreAbility->GetActivationGroup(), CoreAbility);
	}
}

void UCoreAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags)
{
	FGameplayTagContainer ModifiedBlockTags = BlockTags;
	FGameplayTagContainer ModifiedCancelTags = CancelTags;

	if (TagRelationshipMapping)
	{
		// Use the mapping to expand the ability tags into block and cancel tag
		TagRelationshipMapping->GetAbilityTagsToBlockAndCancel(AbilityTags, &ModifiedBlockTags, &ModifiedCancelTags);
	}

	Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, ModifiedBlockTags, bExecuteCancelTags, ModifiedCancelTags);

	//@TODO: Apply any special logic like blocking input or movement
}

void UCoreAbilitySystemComponent::HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bCanBeCanceled)
{
	Super::HandleChangeAbilityCanBeCanceled(AbilityTags, RequestingAbility, bCanBeCanceled);

	//@TODO: Apply any special logic like blocking input or movement
}

void UCoreAbilitySystemComponent::GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const
{
	if (TagRelationshipMapping)
	{
		TagRelationshipMapping->GetRequiredAndBlockedActivationTags(AbilityTags, &OutActivationRequired, &OutActivationBlocked);
	}
}

void UCoreAbilitySystemComponent::SetTagRelationshipMapping(UAbilityTagRelationshipMapping* NewMapping)
{
	TagRelationshipMapping = NewMapping;
}

void UCoreAbilitySystemComponent::SetTagRelationshipMapping(UObject* Mapping)
{
	SetTagRelationshipMapping(Cast<UAbilityTagRelationshipMapping>(Mapping));
}

void UCoreAbilitySystemComponent::ClientNotifyAbilityFailed_Implementation(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	HandleAbilityFailed(Ability, FailureReason);
}

void UCoreAbilitySystemComponent::HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	UE_LOG(LogTemp, Warning, TEXT("Ability %s failed to activate (tags: %s)"), *GetPathNameSafe(Ability), *FailureReason.ToString());

	if (const UCoreGameplayAbility* CoreAbility = Cast<const UCoreGameplayAbility>(Ability))
	{
		CoreAbility->OnAbilityFailedToActivate(FailureReason);
	}
}

bool UCoreAbilitySystemComponent::IsActivationGroupBlocked(EAbilityActivationGroup Group) const
{
	bool bBlocked = false;

	switch (Group)
	{
	case EAbilityActivationGroup::Independent:
		// Independent abilities are never blocked.
		bBlocked = false;
		break;

	case EAbilityActivationGroup::Exclusive_Replaceable:
	case EAbilityActivationGroup::Exclusive_Blocking:
		// Exclusive abilities can activate if nothing is blocking.
		bBlocked = (ActivationGroupCounts[(uint8)EAbilityActivationGroup::Exclusive_Blocking] > 0);
		break;

	default:
		checkf(false, TEXT("IsActivationGroupBlocked: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	return bBlocked;
}

void UCoreAbilitySystemComponent::AddAbilityToActivationGroup(EAbilityActivationGroup Group, UCoreGameplayAbility* CoreAbility)
{
	check(CoreAbility);
	check(ActivationGroupCounts[(uint8)Group] < INT32_MAX);

	ActivationGroupCounts[(uint8)Group]++;

	const bool bReplicateCancelAbility = false;

	switch (Group)
	{
	case EAbilityActivationGroup::Independent:
		// Independent abilities do not cancel any other abilities.
		break;

	case EAbilityActivationGroup::Exclusive_Replaceable:
	case EAbilityActivationGroup::Exclusive_Blocking:
		CancelActivationGroupAbilities(EAbilityActivationGroup::Exclusive_Replaceable, CoreAbility, bReplicateCancelAbility);
		break;

	default:
		checkf(false, TEXT("AddAbilityToActivationGroup: Invalid ActivationGroup [%d]\n"), (uint8)Group);
		break;
	}

	const int32 ExclusiveCount = ActivationGroupCounts[(uint8)EAbilityActivationGroup::Exclusive_Replaceable] + ActivationGroupCounts[(uint8)EAbilityActivationGroup::Exclusive_Blocking];
	if (!ensure(ExclusiveCount <= 1))
	{
		//UE_LOG(LogLyraAbilitySystem, Error, TEXT("AddAbilityToActivationGroup: Multiple exclusive abilities are running."));
	}
}

void UCoreAbilitySystemComponent::RemoveAbilityFromActivationGroup(EAbilityActivationGroup Group, UCoreGameplayAbility* CoreAbility)
{
	check(CoreAbility);
	check(ActivationGroupCounts[(uint8)Group] > 0);

	ActivationGroupCounts[(uint8)Group]--;
}

void UCoreAbilitySystemComponent::CancelActivationGroupAbilities(EAbilityActivationGroup Group, UCoreGameplayAbility* IgnoreCoreAbility, bool bReplicateCancelAbility)
{
	auto ShouldCancelFunc = [this, Group, IgnoreCoreAbility](const UCoreGameplayAbility* CoreAbility, FGameplayAbilitySpecHandle Handle)
		{
			return ((CoreAbility->GetActivationGroup() == Group) && (CoreAbility != IgnoreCoreAbility));
		};

	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
}

void UCoreAbilitySystemComponent::AddDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
	//const TSubclassOf<UGameplayEffect> DynamicTagGE = ULyraAssetManager::GetSubclass(ULyraGameData::Get().DynamicTagGameplayEffect);
	//if (!DynamicTagGE)
	//{
	//	UE_LOG(LogLyraAbilitySystem, Warning, TEXT("AddDynamicTagGameplayEffect: Unable to find DynamicTagGameplayEffect [%s]."), *ULyraGameData::Get().DynamicTagGameplayEffect.GetAssetName());
	//	return;
	//}

	//const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(DynamicTagGE, 1.0f, MakeEffectContext());
	//FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

	//if (!Spec)
	//{
	//	UE_LOG(LogLyraAbilitySystem, Warning, TEXT("AddDynamicTagGameplayEffect: Unable to make outgoing spec for [%s]."), *GetNameSafe(DynamicTagGE));
	//	return;
	//}

	//Spec->DynamicGrantedTags.AddTag(Tag);

	//ApplyGameplayEffectSpecToSelf(*Spec);
}

void UCoreAbilitySystemComponent::RemoveDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
	//const TSubclassOf<UGameplayEffect> DynamicTagGE = ULyraAssetManager::GetSubclass(ULyraGameData::Get().DynamicTagGameplayEffect);
	//if (!DynamicTagGE)
	//{
	//	UE_LOG(LogLyraAbilitySystem, Warning, TEXT("RemoveDynamicTagGameplayEffect: Unable to find gameplay effect [%s]."), *ULyraGameData::Get().DynamicTagGameplayEffect.GetAssetName());
	//	return;
	//}

	//FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(Tag));
	//Query.EffectDefinition = DynamicTagGE;

	//RemoveActiveEffects(Query);
}

void UCoreAbilitySystemComponent::GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle)
{
	TSharedPtr<FAbilityReplicatedDataCache> ReplicatedData = AbilityTargetDataMap.Find(FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, ActivationInfo.GetActivationPredictionKey()));
	if (ReplicatedData.IsValid())
	{
		OutTargetDataHandle = ReplicatedData->TargetData;
	}
}

