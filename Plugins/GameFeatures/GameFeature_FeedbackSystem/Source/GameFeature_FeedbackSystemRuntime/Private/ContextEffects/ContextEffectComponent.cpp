// Copyright Epic Games, Inc. All Rights Reserved.

#include "ContextEffects/ContextEffectComponent.h"

#include "ContextEffects/ContextEffectsSubsystem.h"
#include "Engine/World.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ContextEffectComponent)

class UAnimSequenceBase;
class USceneComponent;

UContextEffectComponent::UContextEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
}

void UContextEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentContexts.AppendTags(DefaultEffectContexts);
	CurrentContextEffectsLibraries = DefaultContextEffectsLibraries;

	if (const UWorld* World = GetWorld())
	{
		if (UContextEffectsSubsystem* ContextEffectsSubsystem = World->GetSubsystem<UContextEffectsSubsystem>())
		{
			ContextEffectsSubsystem->LoadAndAddContextEffectsLibraries(GetOwner(), CurrentContextEffectsLibraries);
		}
	}
}

void UContextEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const UWorld* World = GetWorld())
	{
		if (UContextEffectsSubsystem* ContextEffectsSubsystem = World->GetSubsystem<UContextEffectsSubsystem>())
		{
			ContextEffectsSubsystem->UnloadAndRemoveContextEffectsLibraries(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UContextEffectComponent::AnimMotionEffect_Implementation(
	const FName Bone,
	const FGameplayTag MotionEffect,
	USceneComponent* StaticMeshComponent,
	const FVector LocationOffset,
	const FRotator RotationOffset,
	const UAnimSequenceBase* AnimationSequence,
	const bool bHitSuccess,
	const FHitResult HitResult,
	FGameplayTagContainer Contexts,
	FVector VFXScale,
	float AudioVolume,
	float AudioPitch)
{
	TArray<UAudioComponent*> AudioComponentsToAdd;
	TArray<UNiagaraComponent*> NiagaraComponentsToAdd;

	FGameplayTagContainer TotalContexts;
	TotalContexts.AppendTags(Contexts);
	TotalContexts.AppendTags(CurrentContexts);

	if (bConvertPhysicalSurfaceToContext)
	{
		TWeakObjectPtr<UPhysicalMaterial> PhysicalSurfaceTypePtr = HitResult.PhysMaterial;

		if (PhysicalSurfaceTypePtr.IsValid())
		{
			TEnumAsByte<EPhysicalSurface> PhysicalSurfaceType = PhysicalSurfaceTypePtr->SurfaceType;

			if (const UContextEffectsSettings* ContextEffectsSettings = GetDefault<UContextEffectsSettings>())
			{
				if (const FGameplayTag* SurfaceContextPtr = ContextEffectsSettings->SurfaceTypeToContextMap.Find(PhysicalSurfaceType))
				{
					TotalContexts.AddTag(*SurfaceContextPtr);
				}
			}
		}
	}

	for (UAudioComponent* ActiveAudioComponent : ActiveAudioComponents)
	{
		if (ActiveAudioComponent)
		{
			AudioComponentsToAdd.Add(ActiveAudioComponent);
		}
	}

	for (UNiagaraComponent* ActiveNiagaraComponent : ActiveNiagaraComponents)
	{
		if (ActiveNiagaraComponent)
		{
			NiagaraComponentsToAdd.Add(ActiveNiagaraComponent);
		}
	}

	if (const UWorld* World = GetWorld())
	{
		if (UContextEffectsSubsystem* ContextEffectsSubsystem = World->GetSubsystem<UContextEffectsSubsystem>())
		{
			TArray<UAudioComponent*> AudioComponents;
			TArray<UNiagaraComponent*> NiagaraComponents;

			ContextEffectsSubsystem->SpawnContextEffects(GetOwner(), StaticMeshComponent, Bone,
				LocationOffset, RotationOffset, MotionEffect, TotalContexts,
				AudioComponents, NiagaraComponents, VFXScale, AudioVolume, AudioPitch);

			AudioComponentsToAdd.Append(AudioComponents);
			NiagaraComponentsToAdd.Append(NiagaraComponents);
		}
	}

	ActiveAudioComponents.Empty();
	ActiveAudioComponents.Append(AudioComponentsToAdd);

	ActiveNiagaraComponents.Empty();
	ActiveNiagaraComponents.Append(NiagaraComponentsToAdd);
}

void UContextEffectComponent::UpdateEffectContexts(FGameplayTagContainer NewEffectContexts)
{
	CurrentContexts.Reset(NewEffectContexts.Num());
	CurrentContexts.AppendTags(NewEffectContexts);
}

void UContextEffectComponent::UpdateLibraries(TSet<TSoftObjectPtr<UContextEffectsLibrary>> NewContextEffectsLibraries)
{
	CurrentContextEffectsLibraries = NewContextEffectsLibraries;

	if (const UWorld* World = GetWorld())
	{
		if (UContextEffectsSubsystem* ContextEffectsSubsystem = World->GetSubsystem<UContextEffectsSubsystem>())
		{
			ContextEffectsSubsystem->LoadAndAddContextEffectsLibraries(GetOwner(), CurrentContextEffectsLibraries);
		}
	}
}
