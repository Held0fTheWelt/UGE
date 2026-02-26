// Copyright Epic Games, Inc. All Rights Reserved.

#include "ContextEffects/ContextEffectsSubsystem.h"

#include "ContextEffects/ContextEffectsLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ContextEffectsSubsystem)

class AActor;
class UAudioComponent;
class UNiagaraSystem;
class USceneComponent;
class USoundBase;

void UContextEffectsSubsystem::SpawnContextEffects(
	const AActor* SpawningActor,
	USceneComponent* AttachToComponent,
	const FName AttachPoint,
	const FVector LocationOffset,
	const FRotator RotationOffset,
	FGameplayTag Effect,
	FGameplayTagContainer Contexts,
	TArray<UAudioComponent*>& AudioOut,
	TArray<UNiagaraComponent*>& NiagaraOut,
	FVector VFXScale,
	float AudioVolume,
	float AudioPitch)
{
	if (TObjectPtr<UContextEffectsSet>* EffectsLibrariesSetPtr = ActiveActorEffectsMap.Find(SpawningActor))
	{
		if (UContextEffectsSet* EffectsLibraries = *EffectsLibrariesSetPtr)
		{
			TArray<USoundBase*> TotalSounds;
			TArray<UNiagaraSystem*> TotalNiagaraSystems;

			for (UContextEffectsLibrary* EffectLibrary : EffectsLibraries->ContextEffectsLibraries)
			{
				if (EffectLibrary && EffectLibrary->GetContextEffectsLibraryLoadState() == EContextEffectsLibraryLoadState::Loaded)
				{
					TArray<USoundBase*> Sounds;
					TArray<UNiagaraSystem*> NiagaraSystems;

					EffectLibrary->GetEffects(Effect, Contexts, Sounds, NiagaraSystems);

					TotalSounds.Append(Sounds);
					TotalNiagaraSystems.Append(NiagaraSystems);
				}
				else if (EffectLibrary && EffectLibrary->GetContextEffectsLibraryLoadState() == EContextEffectsLibraryLoadState::Unloaded)
				{
					EffectLibrary->LoadEffects();
				}
			}

			for (USoundBase* Sound : TotalSounds)
			{
				UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(Sound, AttachToComponent, AttachPoint, LocationOffset, RotationOffset, EAttachLocation::KeepRelativeOffset,
					false, AudioVolume, AudioPitch, 0.0f, nullptr, nullptr, true);

				AudioOut.Add(AudioComponent);
			}

			for (UNiagaraSystem* NiagaraSystem : TotalNiagaraSystems)
			{
				UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, AttachToComponent, AttachPoint, LocationOffset,
					RotationOffset, VFXScale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true, true);

				NiagaraOut.Add(NiagaraComponent);
			}
		}
	}
}

bool UContextEffectsSubsystem::GetContextFromSurfaceType(TEnumAsByte<EPhysicalSurface> PhysicalSurface, FGameplayTag& Context)
{
	if (const UContextEffectsSettings* ProjectSettings = GetDefault<UContextEffectsSettings>())
	{
		if (const FGameplayTag* GameplayTagPtr = ProjectSettings->SurfaceTypeToContextMap.Find(PhysicalSurface))
		{
			Context = *GameplayTagPtr;
		}
	}

	return Context.IsValid();
}

void UContextEffectsSubsystem::LoadAndAddContextEffectsLibraries(AActor* OwningActor, TSet<TSoftObjectPtr<UContextEffectsLibrary>> ContextEffectsLibraries)
{
	if (OwningActor == nullptr || ContextEffectsLibraries.Num() <= 0)
	{
		return;
	}

	UContextEffectsSet* EffectsLibrariesSet = NewObject<UContextEffectsSet>(this);

	for (const TSoftObjectPtr<UContextEffectsLibrary>& ContextEffectSoftObj : ContextEffectsLibraries)
	{
		if (UContextEffectsLibrary* EffectsLibrary = ContextEffectSoftObj.LoadSynchronous())
		{
			EffectsLibrary->LoadEffects();
			EffectsLibrariesSet->ContextEffectsLibraries.Add(EffectsLibrary);
		}
	}

	ActiveActorEffectsMap.Emplace(OwningActor, EffectsLibrariesSet);
}

void UContextEffectsSubsystem::UnloadAndRemoveContextEffectsLibraries(AActor* OwningActor)
{
	if (OwningActor == nullptr)
	{
		return;
	}

	ActiveActorEffectsMap.Remove(OwningActor);
}
