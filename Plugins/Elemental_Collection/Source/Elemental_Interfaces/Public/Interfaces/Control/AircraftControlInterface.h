// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AircraftControlInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAircraftControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class ELEMENTAL_INTERFACES_API IAircraftControlInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void StickInput(const FVector2D& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void StickInputKey(int32 Index, float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void RudderInput(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void RudderInputKey(int32 Index, float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void FlapsInput(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void FlapsKey(bool Extend);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void LandingGearKey();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void BrakeKey(bool Engaged);
};
