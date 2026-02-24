// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ShipSetupInterface.generated.h"

UINTERFACE(MinimalAPI)
class UShipSetupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class SHIPS_API IShipSetupInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void SetSpeed(float Speed);
};
