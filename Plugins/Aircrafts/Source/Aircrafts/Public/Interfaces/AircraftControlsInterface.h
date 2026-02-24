// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AircraftControlsInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentStick, const FVector2D&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentRudder, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAircraftThrottle, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentFlaps, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentLandingGear, bool, IsExtended);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentBrake, bool, IsEngaged);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAircraftControlsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class AIRCRAFTS_API IAircraftControlsInterface
{
	GENERATED_BODY()

public:
	virtual FNotifyCurrentStick& GetNotifyCurrentStick() = 0;
	virtual FNotifyCurrentRudder& GetNotifyCurrentRudder() = 0;
	virtual FNotifyCurrentAircraftThrottle& GetNotifyCurrentAircraftThrottle() = 0;
	virtual FNotifyCurrentFlaps& GetNotifyCurrentFlaps() = 0;
	virtual FNotifyCurrentLandingGear& GetNotifyCurrentLandingGear() = 0;
	virtual FNotifyCurrentBrake& GetNotifyCurrentBrake() = 0;
};
