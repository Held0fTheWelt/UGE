// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInputControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INPUTSYSTEM_API IInputControllerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Configuration")
	EInputConfiguration GetInputConfiguration() const;
};
