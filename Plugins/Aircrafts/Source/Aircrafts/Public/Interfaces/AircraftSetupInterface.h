// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AircraftSetupInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAircraftSetupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AIRCRAFTS_API IAircraftSetupInterface
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void SetSpeed(float Speed);
};
