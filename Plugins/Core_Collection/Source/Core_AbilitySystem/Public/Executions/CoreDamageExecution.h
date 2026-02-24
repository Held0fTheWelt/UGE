// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "GameplayEffectExecutionCalculation.h"

#include "CoreDamageExecution.generated.h"

class UObject;


/**
 * UCoreDamageExecution
 *
 *	Execution used by gameplay effects to apply damage to the health attributes.
 * Adapted from ULyraDamageExecution.
 */
UCLASS()
class CORE_ABILITYSYSTEM_API UCoreDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UCoreDamageExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
