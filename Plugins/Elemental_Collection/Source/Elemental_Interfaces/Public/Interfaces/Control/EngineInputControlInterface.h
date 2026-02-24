// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EngineInputControlInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEngineInputControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API IEngineInputControlInterface
{
	GENERATED_BODY()

public:
	/** Called for throttle input (e.g. accelerator pedal, 0..+1) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle Actions")
	void Throttle(float Value);
};
