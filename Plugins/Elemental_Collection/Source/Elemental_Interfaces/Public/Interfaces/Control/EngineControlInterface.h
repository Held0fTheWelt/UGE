// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EngineControlInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyEngineActiveState, bool, IsOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifySecondEngineActiveState, bool, IsOn);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEngineControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API IEngineControlInterface
{
	GENERATED_BODY()

public:
	/** Called when the engine input event started */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void StartEngineSwitch();
	/** Called when the engine input event stopped */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void StopEngineSwitch();
	/** Called when the second engine input event started */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void StartSecondEngineSwitch();
	/** Called when the second engine input event stopped */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void StopSecondEngineSwitch();

	/** Notify about the engine active state change */
	virtual FNotifyEngineActiveState& GetNotifyEngineActiveState() = 0;
	/** Notify about the second engine active state change */
	virtual FNotifySecondEngineActiveState& GetNotifySecondEngineActiveState() = 0;
};
