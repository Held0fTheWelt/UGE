// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SecondarySystem_AltitudeAndSpeedInterface.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentPLV, const FVector&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentHorizontalSpeed, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentVerticalSpeed, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAltitudeAboveSealevel, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAltitudeAboveGround, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentYaw, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentPitch, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentRoll, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentPredictedLocation, const FVector&, Value);
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USecondarySystem_AltitudeAndSpeedInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CORE_INTERFACES_API ISecondarySystem_AltitudeAndSpeedInterface
{
	GENERATED_BODY()

public:
	/** Called to set the current physics linear velocity */
	virtual FNotifyCurrentPLV& GetNotifyCurrentPLV() = 0;
	/** Called to set the current horizontal speed */
	virtual FNotifyCurrentHorizontalSpeed& GetNotifyCurrentHorizontalSpeed() = 0;
	/** Called to set the current vertical speed */
	virtual FNotifyCurrentVerticalSpeed& GetNotifyCurrentVerticalSpeed() = 0;
	/** Called to set the current altitude above sea level */
	virtual FNotifyCurrentAltitudeAboveSealevel& GetNotifyCurrentAltitudeAboveSealevel() = 0;
	/** Called to set the current altitude above ground */
	virtual FNotifyCurrentAltitudeAboveGround& GetNotifyCurrentAltitudeAboveGround() = 0;
	/** Called to set the current yaw angle */
	virtual FNotifyCurrentYaw& GetNotifyCurrentYaw() = 0;
	/** Called to set the current pitch angle */
	virtual FNotifyCurrentPitch& GetNotifyCurrentPitch() = 0;
	/** Called to set the current roll angle */
	virtual FNotifyCurrentRoll& GetNotifyCurrentRoll() = 0;
	/** Called to set the predicted location based on current velocity and DeltaTime */
	virtual FNotifyCurrentPredictedLocation& GetNotifyCurrentPredictedLocation() = 0;
};
