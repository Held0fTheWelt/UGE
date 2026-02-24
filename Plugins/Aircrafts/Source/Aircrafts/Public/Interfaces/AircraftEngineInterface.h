// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AircraftEngineInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAircraftThrustForce, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAircraftEngineRPM, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAircraftFuelFlow, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAircraftLinearDamping, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAircraftTorqueReduction, float, Value);

UINTERFACE(MinimalAPI)
class UAircraftEngineInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class AIRCRAFTS_API IAircraftEngineInterface
{
	GENERATED_BODY()

public:
	/** Called to broadcast the computed thrust force in N */
	virtual FNotifyCurrentAircraftThrustForce& GetNotifyCurrentAircraftThrustForce() = 0;
	/** Called to broadcast the engine RPM for gauges/sound */
	virtual FNotifyCurrentAircraftEngineRPM& GetNotifyCurrentAircraftEngineRPM() = 0;
	/** Called to broadcast the fuel consumption rate (kg/h) */
	virtual FNotifyCurrentAircraftFuelFlow& GetNotifyCurrentAircraftFuelFlow() = 0;
	/** Called to broadcast the linear damping value (brakes/gear) */
	virtual FNotifyCurrentAircraftLinearDamping& GetNotifyCurrentAircraftLinearDamping() = 0;
	/** Called to broadcast the yaw authority reduction at speed */
	virtual FNotifyCurrentAircraftTorqueReduction& GetNotifyCurrentAircraftTorqueReduction() = 0;
};
