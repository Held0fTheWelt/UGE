// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Phases/CoreGamePhaseSubsystem.h"

#include "Components/CoreAbilitySystemComponent.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "Phases/CoreGamePhaseAbility.h"
#include "Phases/CoreGamePhaseLog.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreGamePhaseSubsystem)

//////////////////////////////////////////////////////////////////////
// UCoreGamePhaseSubsystem

UCoreGamePhaseSubsystem::UCoreGamePhaseSubsystem()
{
}

void UCoreGamePhaseSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

bool UCoreGamePhaseSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (Super::ShouldCreateSubsystem(Outer))
	{
		return true;
	}

	return false;
}

bool UCoreGamePhaseSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UCoreGamePhaseSubsystem::StartPhase(TSubclassOf<UCoreGamePhaseAbility> PhaseAbility, FCoreGamePhaseDelegate PhaseEndedCallback)
{
	UWorld* World = GetWorld();
	UCoreAbilitySystemComponent* GameState_ASC = World->GetGameState()->FindComponentByClass<UCoreAbilitySystemComponent>();
	if (ensure(GameState_ASC))
	{
		FGameplayAbilitySpec PhaseSpec(PhaseAbility, 1, 0, this);
		FGameplayAbilitySpecHandle SpecHandle = GameState_ASC->GiveAbilityAndActivateOnce(PhaseSpec);
		FGameplayAbilitySpec* FoundSpec = GameState_ASC->FindAbilitySpecFromHandle(SpecHandle);
		
		if (FoundSpec && FoundSpec->IsActive())
		{
			FCoreGamePhaseEntry& Entry = ActivePhaseMap.FindOrAdd(SpecHandle);
			Entry.PhaseEndedCallback = PhaseEndedCallback;
		}
		else
		{
			PhaseEndedCallback.ExecuteIfBound(nullptr);
		}
	}
}

void UCoreGamePhaseSubsystem::K2_StartPhase(TSubclassOf<UCoreGamePhaseAbility> PhaseAbility, const FCoreGamePhaseDynamicDelegate& PhaseEndedDelegate)
{
	const FCoreGamePhaseDelegate EndedDelegate = FCoreGamePhaseDelegate::CreateWeakLambda(const_cast<UObject*>(PhaseEndedDelegate.GetUObject()), [PhaseEndedDelegate](const UCoreGamePhaseAbility* PhaseAbility) {
		PhaseEndedDelegate.ExecuteIfBound(PhaseAbility);
	});

	StartPhase(PhaseAbility, EndedDelegate);
}

void UCoreGamePhaseSubsystem::K2_WhenPhaseStartsOrIsActive(FGameplayTag PhaseTag, ECorePhaseTagMatchType MatchType, FCoreGamePhaseTagDynamicDelegate WhenPhaseActive)
{
	const FCoreGamePhaseTagDelegate ActiveDelegate = FCoreGamePhaseTagDelegate::CreateWeakLambda(WhenPhaseActive.GetUObject(), [WhenPhaseActive](const FGameplayTag& PhaseTag) {
		WhenPhaseActive.ExecuteIfBound(PhaseTag);
	});

	WhenPhaseStartsOrIsActive(PhaseTag, MatchType, ActiveDelegate);
}

void UCoreGamePhaseSubsystem::K2_WhenPhaseEnds(FGameplayTag PhaseTag, ECorePhaseTagMatchType MatchType, FCoreGamePhaseTagDynamicDelegate WhenPhaseEnd)
{
	const FCoreGamePhaseTagDelegate EndedDelegate = FCoreGamePhaseTagDelegate::CreateWeakLambda(WhenPhaseEnd.GetUObject(), [WhenPhaseEnd](const FGameplayTag& PhaseTag) {
		WhenPhaseEnd.ExecuteIfBound(PhaseTag);
	});

	WhenPhaseEnds(PhaseTag, MatchType, EndedDelegate);
}

void UCoreGamePhaseSubsystem::WhenPhaseStartsOrIsActive(FGameplayTag PhaseTag, ECorePhaseTagMatchType MatchType, const FCoreGamePhaseTagDelegate& WhenPhaseActive)
{
	FPhaseObserver Observer;
	Observer.PhaseTag = PhaseTag;
	Observer.MatchType = MatchType;
	Observer.PhaseCallback = WhenPhaseActive;
	PhaseStartObservers.Add(Observer);

	if (IsPhaseActive(PhaseTag))
	{
		WhenPhaseActive.ExecuteIfBound(PhaseTag);
	}
}

void UCoreGamePhaseSubsystem::WhenPhaseEnds(FGameplayTag PhaseTag, ECorePhaseTagMatchType MatchType, const FCoreGamePhaseTagDelegate& WhenPhaseEnd)
{
	FPhaseObserver Observer;
	Observer.PhaseTag = PhaseTag;
	Observer.MatchType = MatchType;
	Observer.PhaseCallback = WhenPhaseEnd;
	PhaseEndObservers.Add(Observer);
}

bool UCoreGamePhaseSubsystem::IsPhaseActive(const FGameplayTag& PhaseTag) const
{
	for (const auto& KVP : ActivePhaseMap)
	{
		const FCoreGamePhaseEntry& PhaseEntry = KVP.Value;
		if (PhaseEntry.PhaseTag.MatchesTag(PhaseTag))
		{
			return true;
		}
	}

	return false;
}

void UCoreGamePhaseSubsystem::OnBeginPhase(const UCoreGamePhaseAbility* PhaseAbility, const FGameplayAbilitySpecHandle PhaseAbilityHandle)
{
	const FGameplayTag IncomingPhaseTag = PhaseAbility->GetGamePhaseTag();

	UE_LOG(LogCoreGamePhase, Log, TEXT("Beginning Phase '%s' (%s)"), *IncomingPhaseTag.ToString(), *GetNameSafe(PhaseAbility));

	const UWorld* World = GetWorld();
	UCoreAbilitySystemComponent* GameState_ASC = World->GetGameState()->FindComponentByClass<UCoreAbilitySystemComponent>();
	if (ensure(GameState_ASC))
	{
		TArray<FGameplayAbilitySpec*> ActivePhases;
		for (const auto& KVP : ActivePhaseMap)
		{
			const FGameplayAbilitySpecHandle ActiveAbilityHandle = KVP.Key;
			if (FGameplayAbilitySpec* Spec = GameState_ASC->FindAbilitySpecFromHandle(ActiveAbilityHandle))
			{
				ActivePhases.Add(Spec);
			}
		}

		for (const FGameplayAbilitySpec* ActivePhase : ActivePhases)
		{
			const UCoreGamePhaseAbility* ActivePhaseAbility = CastChecked<UCoreGamePhaseAbility>(ActivePhase->Ability);
			const FGameplayTag ActivePhaseTag = ActivePhaseAbility->GetGamePhaseTag();
			
			// So if the active phase currently matches the incoming phase tag, we allow it.
			// i.e. multiple gameplay abilities can all be associated with the same phase tag.
			// For example,
			// You can be in the, Game.Playing, phase, and then start a sub-phase, like Game.Playing.SuddenDeath
			// Game.Playing phase will still be active, and if someone were to push another one, like,
			// Game.Playing.ActualSuddenDeath, it would end Game.Playing.SuddenDeath phase, but Game.Playing would
			// continue.  Similarly if we activated Game.GameOver, all the Game.Playing* phases would end.
			if (!IncomingPhaseTag.MatchesTag(ActivePhaseTag))
			{
				UE_LOG(LogCoreGamePhase, Log, TEXT("\tEnding Phase '%s' (%s)"), *ActivePhaseTag.ToString(), *GetNameSafe(ActivePhaseAbility));

				FGameplayAbilitySpecHandle HandleToEnd = ActivePhase->Handle;
				GameState_ASC->CancelAbilitiesByFunc([HandleToEnd](const UCoreGameplayAbility* CoreAbility, FGameplayAbilitySpecHandle Handle) {
					return Handle == HandleToEnd;
				}, true);
			}
		}

		FCoreGamePhaseEntry& Entry = ActivePhaseMap.FindOrAdd(PhaseAbilityHandle);
		Entry.PhaseTag = IncomingPhaseTag;

		// Notify all observers of this phase that it has started.
		for (const FPhaseObserver& Observer : PhaseStartObservers)
		{
			if (Observer.IsMatch(IncomingPhaseTag))
			{
				Observer.PhaseCallback.ExecuteIfBound(IncomingPhaseTag);
			}
		}
	}
}

void UCoreGamePhaseSubsystem::OnEndPhase(const UCoreGamePhaseAbility* PhaseAbility, const FGameplayAbilitySpecHandle PhaseAbilityHandle)
{
	const FGameplayTag EndedPhaseTag = PhaseAbility->GetGamePhaseTag();
	UE_LOG(LogCoreGamePhase, Log, TEXT("Ended Phase '%s' (%s)"), *EndedPhaseTag.ToString(), *GetNameSafe(PhaseAbility));

	const FCoreGamePhaseEntry& Entry = ActivePhaseMap.FindChecked(PhaseAbilityHandle);
	Entry.PhaseEndedCallback.ExecuteIfBound(PhaseAbility);

	ActivePhaseMap.Remove(PhaseAbilityHandle);

	// Notify all observers of this phase that it has ended.
	for (const FPhaseObserver& Observer : PhaseEndObservers)
	{
		if (Observer.IsMatch(EndedPhaseTag))
		{
			Observer.PhaseCallback.ExecuteIfBound(EndedPhaseTag);
		}
	}
}

bool UCoreGamePhaseSubsystem::FPhaseObserver::IsMatch(const FGameplayTag& ComparePhaseTag) const
{
	switch(MatchType)
	{
	case ECorePhaseTagMatchType::ExactMatch:
		return ComparePhaseTag == PhaseTag;
	case ECorePhaseTagMatchType::PartialMatch:
		return ComparePhaseTag.MatchesTag(PhaseTag);
	}

	return false;
}
