// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "ElementalInitStateTagSubsystem.generated.h"

/**
 * World subsystem that provides init state gameplay tags for ModularGameplay (e.g. InitState.Spawned, DataAvailable, ...).
 * Core_Collection (or the game) sets these tags at startup via SetInitStateTags; GameFeatures like HeroSystem read them
 * and do not depend on Core_Structure for tag definitions.
 */
UCLASS()
class ELEMENTAL_COMMON_API UElementalInitStateTagSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Called by Core/game at startup to provide init state tags. HeroSystem and similar use these without depending on Core. */
	UFUNCTION(BlueprintCallable, Category = "Elemental|InitState")
	void SetInitStateTags(FGameplayTag InSpawned, FGameplayTag InDataAvailable, FGameplayTag InDataInitialized, FGameplayTag InGameplayReady);

	/** Feature name for the pawn extension component (e.g. "PawnExtension"). Set by Core so HeroSystem can use HasFeatureReachedInitState without depending on Core. */
	UFUNCTION(BlueprintCallable, Category = "Elemental|InitState")
	void SetPawnExtensionFeatureName(FName InName) { PawnExtensionFeatureName = InName; }

	UFUNCTION(BlueprintPure, Category = "Elemental|InitState")
	FGameplayTag GetInitStateSpawned() const { return InitState_Spawned; }
	UFUNCTION(BlueprintPure, Category = "Elemental|InitState")
	FGameplayTag GetInitStateDataAvailable() const { return InitState_DataAvailable; }
	UFUNCTION(BlueprintPure, Category = "Elemental|InitState")
	FGameplayTag GetInitStateDataInitialized() const { return InitState_DataInitialized; }
	UFUNCTION(BlueprintPure, Category = "Elemental|InitState")
	FGameplayTag GetInitStateGameplayReady() const { return InitState_GameplayReady; }
	UFUNCTION(BlueprintPure, Category = "Elemental|InitState")
	FName GetPawnExtensionFeatureName() const { return PawnExtensionFeatureName; }

private:
	UPROPERTY()
	FGameplayTag InitState_Spawned;
	UPROPERTY()
	FGameplayTag InitState_DataAvailable;
	UPROPERTY()
	FGameplayTag InitState_DataInitialized;
	UPROPERTY()
	FGameplayTag InitState_GameplayReady;
	FName PawnExtensionFeatureName;
};
