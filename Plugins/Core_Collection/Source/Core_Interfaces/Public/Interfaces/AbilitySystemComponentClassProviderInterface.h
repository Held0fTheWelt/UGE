// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilitySystemComponentClassProviderInterface.generated.h"

class UClass;

/**
 * Interface for providing the AbilitySystemComponent class to use for game state / character.
 * Used to break the dependency of Core_Classes on Core_AbilitySystem.
 * Core_AbilitySystem registers its class at module startup.
 */
UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UAbilitySystemComponentClassProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class CORE_INTERFACES_API IAbilitySystemComponentClassProviderInterface
{
	GENERATED_BODY()

public:
	/** Returns the UAbilitySystemComponent-derived class to use (e.g. UCoreAbilitySystemComponent). */
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	virtual UClass* GetAbilitySystemComponentClass() const { return nullptr; }
};
