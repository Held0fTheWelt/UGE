// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "GameplayEffectExecutionCalculation.h"

#include "CoreHealExecution.generated.h"

class UObject;


/**
 * UCoreHealExecution
 *
 *	Execution used by gameplay effects to apply healing to the health attributes.
 *	Adapted from ULyraHealExecution.
 */
UCLASS()
class CORE_ABILITYSYSTEM_API UCoreHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UCoreHealExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
