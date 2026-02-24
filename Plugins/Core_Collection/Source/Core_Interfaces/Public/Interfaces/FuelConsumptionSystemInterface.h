// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FuelConsumptionSystemInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyFuelLevelAsValue, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyFuelLevelInPercent, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyIsOutOfFuel, bool, IsOutOfFuel);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFuelConsumptionSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CORE_INTERFACES_API IFuelConsumptionSystemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void ConsumeFuel(float FuelAmount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void RefillFuel(float FuelAmount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	float GetMaxFuelLevelAsValue() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	float GetFuelLevelAsValue() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	float GetFuelLevelInPercent() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	bool IsOutOfFuel() const;

	virtual FNotifyFuelLevelAsValue& GetNotifyFuelLevelAsValue() = 0;
	virtual FNotifyFuelLevelInPercent& GetNotifyFuelLevelInPercent() = 0;
	virtual FNotifyIsOutOfFuel& GetNotifyIsOutOfFuel() = 0;
};
