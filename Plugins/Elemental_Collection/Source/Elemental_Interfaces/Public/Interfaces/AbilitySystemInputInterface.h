// Copyright Epic Games, Inc. All Rights Reserved.
// Elemental_Interfaces: Optional extension for ability system components (tag stacks, input processing).
// Implemented by game-specific ASC (e.g. Core); plugins like Controllers use only this interface.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInputInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UAbilitySystemInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Optional interface for ability system components that support loose gameplay tag counts and input processing.
 * Used by Controllers and other plugins that must not depend on Core or GameFeatures.
 * The game's ASC (e.g. CoreAbilitySystemComponent) implements this interface.
 */
class ELEMENTAL_INTERFACES_API IAbilitySystemInputInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	virtual int32 GetTagCount(FGameplayTag Tag) const { return 0; }

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	virtual void SetLooseGameplayTagCount(FGameplayTag Tag, int32 Count) {}

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	virtual void ProcessAbilityInput(float DeltaTime, bool bGamePaused) {}

	/** Optional: activate abilities that should run on spawn (e.g. after replication). Default no-op. */
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	virtual void TryActivateAbilitiesOnSpawn() {}

	/** Notify that an ability input tag was pressed. Used by HeroSystem without depending on Core ASC. */
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	virtual void AbilityInputTagPressed(FGameplayTag InputTag) {}

	/** Notify that an ability input tag was released. Used by HeroSystem without depending on Core ASC. */
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	virtual void AbilityInputTagReleased(FGameplayTag InputTag) {}
};
