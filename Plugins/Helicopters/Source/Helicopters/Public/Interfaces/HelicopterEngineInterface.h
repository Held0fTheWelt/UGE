// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HelicopterEngineInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentMainRotorSpeed, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentTailRotorSpeed, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentLiftValue, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyEngineThrustValue, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentSideForceValue, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentLinearDamping, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAirFriction, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyTorqueReductionSpeed, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentPhysicsLiftVector, const FVector&, Value);

UINTERFACE(MinimalAPI)
class UHelicopterEngineInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELICOPTERS_API IHelicopterEngineInterface
{
	GENERATED_BODY()

public:
	/** Called to set the main rotor speed */
	virtual FNotifyCurrentMainRotorSpeed& GetNotifyCurrentMainRotorSpeed() = 0;
	/** Called to set the tail rotor speed */
	virtual FNotifyCurrentTailRotorSpeed& GetNotifyCurrentTailRotorSpeed() = 0;
	/** Called to set the lift value */
	virtual FNotifyCurrentLiftValue& GetNotifyCurrentLiftValue() = 0;
	/** Called to set the engine thrust value */
	virtual FNotifyEngineThrustValue& GetNotifyCurrentEngineThrust() = 0;
	/** Called to set the side force value */
	virtual FNotifyCurrentSideForceValue& GetNotifyCurrentSideForceValue() = 0;
	/** Called to set the linear damping value */
	virtual FNotifyCurrentLinearDamping& GetNotifyCurrentLinearDamping() = 0;
	/** Called to set the air friction value */
	virtual FNotifyCurrentAirFriction& GetNotifyCurrentAirFriction() = 0;
	/** Called to set the torque reduction speed */
	virtual FNotifyTorqueReductionSpeed& GetNotifyTorqueReductionSpeed() = 0;
	/** Called to set the current physics lift vector */
	virtual FNotifyCurrentPhysicsLiftVector& GetNotifyCurrentPhysicsLiftVector() = 0;

};
