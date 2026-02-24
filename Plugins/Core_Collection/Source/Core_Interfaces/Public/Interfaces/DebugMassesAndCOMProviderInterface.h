// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DebugMassesAndCOMProviderInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UDebugMassesAndCOMProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Components implementing this interface can be toggled to draw debug masses and center-of-mass
 * (e.g. via console command heli.DebugCOM from the Helicopters plugin).
 */
class CORE_INTERFACES_API IDebugMassesAndCOMProviderInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Debug")
	void SetDebugMassesAndCOMEnabled(bool bEnabled);
};
