// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/RuntimeMode.h"
#include "ToggleRuntimeInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UToggleRuntimeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API IToggleRuntimeInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle Actions")
	void ToggleRuntime(ERuntimeMode Mode);
};
