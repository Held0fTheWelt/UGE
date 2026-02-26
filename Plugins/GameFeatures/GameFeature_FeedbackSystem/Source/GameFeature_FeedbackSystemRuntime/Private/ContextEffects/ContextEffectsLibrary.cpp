// Copyright Epic Games, Inc. All Rights Reserved.

#include "ContextEffects/ContextEffectsLibrary.h"

#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ContextEffectsLibrary)

void UContextEffectsLibrary::GetEffects(const FGameplayTag Effect, const FGameplayTagContainer Context,
	TArray<USoundBase*>& Sounds, TArray<UNiagaraSystem*>& NiagaraSystems)
{
	if (Effect.IsValid() && Context.IsValid() && EffectsLoadState == EContextEffectsLibraryLoadState::Loaded)
	{
		for (const auto& ActiveContextEffect : ActiveContextEffects)
		{
			if (Effect.MatchesTagExact(ActiveContextEffect->EffectTag)
				&& Context.HasAllExact(ActiveContextEffect->Context)
				&& (ActiveContextEffect->Context.IsEmpty() == Context.IsEmpty()))
			{
				Sounds.Append(ActiveContextEffect->Sounds);
				NiagaraSystems.Append(ActiveContextEffect->NiagaraSystems);
			}
		}
	}
}

void UContextEffectsLibrary::LoadEffects()
{
	if (EffectsLoadState != EContextEffectsLibraryLoadState::Loading)
	{
		EffectsLoadState = EContextEffectsLibraryLoadState::Loading;

		ActiveContextEffects.Empty();

		LoadEffectsInternal();
	}
}

EContextEffectsLibraryLoadState UContextEffectsLibrary::GetContextEffectsLibraryLoadState()
{
	return EffectsLoadState;
}

void UContextEffectsLibrary::LoadEffectsInternal()
{
	TArray<FContextEffects> LocalContextEffects = ContextEffects;

	TArray<UActiveContextEffects*> ActiveContextEffectsArray;

	for (const FContextEffects& ContextEffect : LocalContextEffects)
	{
		if (ContextEffect.EffectTag.IsValid() && ContextEffect.Context.IsValid())
		{
			UActiveContextEffects* NewActiveContextEffects = NewObject<UActiveContextEffects>(this);

			NewActiveContextEffects->EffectTag = ContextEffect.EffectTag;
			NewActiveContextEffects->Context = ContextEffect.Context;

			for (const FSoftObjectPath& Effect : ContextEffect.Effects)
			{
				if (UObject* Object = Effect.TryLoad())
				{
					if (Object->IsA(USoundBase::StaticClass()))
					{
						if (USoundBase* SoundBase = Cast<USoundBase>(Object))
						{
							NewActiveContextEffects->Sounds.Add(SoundBase);
						}
					}
					else if (Object->IsA(UNiagaraSystem::StaticClass()))
					{
						if (UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(Object))
						{
							NewActiveContextEffects->NiagaraSystems.Add(NiagaraSystem);
						}
					}
				}
			}

			ActiveContextEffectsArray.Add(NewActiveContextEffects);
		}
	}

	ActiveContextEffects = ActiveContextEffectsArray;

	EffectsLoadState = EContextEffectsLibraryLoadState::Loaded;
}
