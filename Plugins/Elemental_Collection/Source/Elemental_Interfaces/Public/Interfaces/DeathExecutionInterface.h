// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DeathExecutionInterface.generated.h"

/** Death state for use by death ability and health components */
UENUM(BlueprintType)
enum class EDeathExecutionState : uint8
{
	NotDead,
	DeathStarted,
	DeathFinished
};

/**
 * Interface for components/actors that can execute death (StartDeath/FinishDeath).
 * Used by CoreGameplayAbility_Death without depending on a concrete Health GameFeature.
 * Health GameFeature (when present) should implement this on its health component.
 */
UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UDeathExecutionInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IDeathExecutionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Death")
	virtual EDeathExecutionState GetDeathState() const { return EDeathExecutionState::NotDead; }

	UFUNCTION(BlueprintCallable, Category = "Death")
	virtual void StartDeath() {}

	UFUNCTION(BlueprintCallable, Category = "Death")
	virtual void FinishDeath() {}
};
