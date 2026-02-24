// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ShipEngineInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentShipPropulsionForce, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentShipEngineRPM, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentShipFuelFlow, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentShipRudderForce, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentShipBowThrusterForce, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentShipLinearDamping, float, Value);

UINTERFACE(MinimalAPI)
class UShipEngineInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class SHIPS_API IShipEngineInterface
{
	GENERATED_BODY()

public:
	/** Called to broadcast the computed propulsion force in N */
	virtual FNotifyCurrentShipPropulsionForce& GetNotifyCurrentShipPropulsionForce() = 0;
	/** Called to broadcast the engine RPM for gauges/sound */
	virtual FNotifyCurrentShipEngineRPM& GetNotifyCurrentShipEngineRPM() = 0;
	/** Called to broadcast the fuel consumption rate (kg/h) */
	virtual FNotifyCurrentShipFuelFlow& GetNotifyCurrentShipFuelFlow() = 0;
	/** Called to broadcast the rudder force from helm + speed */
	virtual FNotifyCurrentShipRudderForce& GetNotifyCurrentShipRudderForce() = 0;
	/** Called to broadcast the bow thruster force */
	virtual FNotifyCurrentShipBowThrusterForce& GetNotifyCurrentShipBowThrusterForce() = 0;
	/** Called to broadcast the linear damping value (anchor/docking) */
	virtual FNotifyCurrentShipLinearDamping& GetNotifyCurrentShipLinearDamping() = 0;
};
