// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "ActiveGameplayEffectHandle.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayAbilitySpecHandle.h"
#include "Templates/SubclassOf.h"

#include "GlobalAbilitySubsystem.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UCoreAbilitySystemComponent;
class UObject;
struct FActiveGameplayEffectHandle;
struct FFrame;
struct FGameplayAbilitySpecHandle;

/**
 * FGlobalAppliedAbilityList
 *
 *	List of globally applied abilities.
 */
USTRUCT()
struct CORE_ABILITYSYSTEM_API FGlobalAppliedAbilityList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UCoreAbilitySystemComponent>, FGameplayAbilitySpecHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayAbility> Ability, UCoreAbilitySystemComponent* ASC);
	void RemoveFromASC(UCoreAbilitySystemComponent* ASC);
	void RemoveFromAll();
};

/**
 * FGlobalAppliedEffectList
 *
 *	List of globally applied effects.
 */
USTRUCT()
struct CORE_ABILITYSYSTEM_API FGlobalAppliedEffectList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UCoreAbilitySystemComponent>, FActiveGameplayEffectHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayEffect> Effect, UCoreAbilitySystemComponent* ASC);
	void RemoveFromASC(UCoreAbilitySystemComponent* ASC);
	void RemoveFromAll();
};

/**
 * UGlobalAbilitySubsystem
 *
 *	World subsystem for managing global abilities and effects.
 *	Adapted from ULyraGlobalAbilitySystem.
 */
UCLASS()
class CORE_ABILITYSYSTEM_API UGlobalAbilitySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UGlobalAbilitySubsystem();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Core|AbilitySystem")
	void ApplyAbilityToAll(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Core|AbilitySystem")
	void ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Core|AbilitySystem")
	void RemoveAbilityFromAll(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Core|AbilitySystem")
	void RemoveEffectFromAll(TSubclassOf<UGameplayEffect> Effect);

	/** Register an ASC with global system and apply any active global effects/abilities. */
	void RegisterASC(UCoreAbilitySystemComponent* ASC);

	/** Removes an ASC from the global system, along with any active global effects/abilities. */
	void UnregisterASC(UCoreAbilitySystemComponent* ASC);

private:
	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGlobalAppliedAbilityList> AppliedAbilities;

	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FGlobalAppliedEffectList> AppliedEffects;

	UPROPERTY()
	TArray<TObjectPtr<UCoreAbilitySystemComponent>> RegisteredASCs;
};
