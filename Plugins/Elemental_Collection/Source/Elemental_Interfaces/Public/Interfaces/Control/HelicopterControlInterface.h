// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HelicopterControlInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHelicopterControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API IHelicopterControlInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void CyclicInput(const FVector2D& Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void CyclicInputKey(int32 Index, float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void CollectiveInput(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void CollectiveKey(bool Triggered);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void PedalsInput(float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void PedalsInputsKey(int32 Index, float Value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void SASToggle();
};
