// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SecondarySystem_FlightAvoidanceInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyHoverInputByAS, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyStrafeInputByAS, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyMFInputByAS, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyMRInputByAS, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyAvoidedByAS, bool, Value);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USecondarySystem_FlightAvoidanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CORE_INTERFACES_API ISecondarySystem_FlightAvoidanceInterface
{
	GENERATED_BODY()

public:
	virtual FNotifyHoverInputByAS& GetNotifyHoverInputByAS() = 0;
	virtual FNotifyStrafeInputByAS& GetNotifyStrafeInputByAS() = 0;
	virtual FNotifyMFInputByAS& GetNotifyMFInputByAS() = 0;
	virtual FNotifyMRInputByAS& GetNotifyMRInputByAS() = 0;
	virtual FNotifyAvoidedByAS& GetNotifyAvoidedByAS() = 0;
};
