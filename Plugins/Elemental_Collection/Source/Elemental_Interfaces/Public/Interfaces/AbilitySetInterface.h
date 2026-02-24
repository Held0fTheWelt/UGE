// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpec.h"
#include "AbilitySetInterface.generated.h"

class UAbilitySystemComponent;
class UObject;

/** Handles for granted abilities/effects; implementation fills these when granting. */
USTRUCT(BlueprintType)
struct ELEMENTAL_INTERFACES_API FAbilitySetGrantedHandles
{
	GENERATED_BODY()

	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	void AddAbilitySpecHandle(FGameplayAbilitySpecHandle Handle) { AbilitySpecHandles.Add(Handle); }
	void AddGameplayEffectHandle(FActiveGameplayEffectHandle Handle) { GameplayEffectHandles.Add(Handle); }

	/** Remove all granted abilities/effects from the ASC. */
	void TakeFromAbilitySystem(class UAbilitySystemComponent* ASC);
};

UINTERFACE(MinimalAPI, BlueprintType)
class UAbilitySetInterface : public UInterface
{
	GENERATED_BODY()
};

/** Interface for assets that grant abilities/effects to an ability system component. */
class ELEMENTAL_INTERFACES_API IAbilitySetInterface
{
	GENERATED_BODY()

public:
	/** Grant abilities/effects from this set to the given ASC; fill InOutHandles with granted handles. */
	virtual void GiveToAbilitySystem(UAbilitySystemComponent* ASC, FAbilitySetGrantedHandles& InOutHandles, UObject* SourceObject = nullptr) = 0;

	/** Remove previously granted abilities/effects that this set added to Handles. */
	virtual void TakeFromAbilitySystem(UAbilitySystemComponent* ASC, FAbilitySetGrantedHandles& Handles) = 0;
};
