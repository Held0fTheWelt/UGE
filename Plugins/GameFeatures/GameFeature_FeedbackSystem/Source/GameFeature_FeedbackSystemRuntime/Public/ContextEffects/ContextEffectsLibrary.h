// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "UObject/SoftObjectPath.h"
#include "UObject/WeakObjectPtr.h"

#include "ContextEffectsLibrary.generated.h"

class UNiagaraSystem;
class USoundBase;
struct FFrame;

/**
 * Load state for context effects library
 */
UENUM()
enum class EContextEffectsLibraryLoadState : uint8
{
	Unloaded = 0,
	Loading = 1,
	Loaded = 2
};

/**
 * Context Effects entry
 */
USTRUCT(BlueprintType)
struct FContextEffects
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Context;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowedClasses = "/Script/Engine.SoundBase, /Script/Niagara.NiagaraSystem"))
	TArray<FSoftObjectPath> Effects;
};

/**
 * Active Context Effects
 */
UCLASS(MinimalAPI)
class UActiveContextEffects : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FGameplayTag EffectTag;

	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainer Context;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<USoundBase>> Sounds;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UNiagaraSystem>> NiagaraSystems;
};

/**
 * Library of Context Effects
 */
UCLASS(MinimalAPI, BlueprintType)
class UContextEffectsLibrary : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FContextEffects> ContextEffects;

	UFUNCTION(BlueprintCallable)
	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API void GetEffects(const FGameplayTag Effect, const FGameplayTagContainer Context, TArray<USoundBase*>& Sounds, TArray<UNiagaraSystem*>& NiagaraSystems);

	UFUNCTION(BlueprintCallable)
	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API void LoadEffects();

	GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API EContextEffectsLibraryLoadState GetContextEffectsLibraryLoadState();

private:
	void LoadEffectsInternal();

	UPROPERTY(Transient)
	TArray<TObjectPtr<UActiveContextEffects>> ActiveContextEffects;

	UPROPERTY(Transient)
	EContextEffectsLibraryLoadState EffectsLoadState = EContextEffectsLibraryLoadState::Unloaded;
};
