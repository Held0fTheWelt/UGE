// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HelicopterControlsInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentCollective, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentCyclic, const FVector2D&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentPedals, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentThrottle, float, Value);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHelicopterControlsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELICOPTERS_API IHelicopterControlsInterface
{
	GENERATED_BODY()

public:
	virtual FNotifyCurrentCollective& GetNotifyCurrentCollective() = 0;
	virtual FNotifyCurrentCyclic& GetNotifyCurrentCyclic() = 0;
	virtual FNotifyCurrentPedals& GetNotifyCurrentPedals() = 0;
	virtual FNotifyCurrentThrottle& GetNotifyCurrentThrottle() = 0;
};
