// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ShipControlInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UShipControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class ELEMENTAL_INTERFACES_API IShipControlInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void HelmInput(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void HelmInputKey(int32 Index, float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void ThrottleInput(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void ThrottleInputKey(int32 Index, float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void BowThrusterInput(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void BowThrusterKey(int32 Index, float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void AnchorKey();
};
