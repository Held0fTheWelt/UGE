// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TurretControlInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTurretControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API ITurretControlInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle Actions")
	void Rotate(const FVector2D& Input);
};
