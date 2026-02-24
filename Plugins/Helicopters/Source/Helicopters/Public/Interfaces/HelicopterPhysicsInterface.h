// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HelicopterPhysicsInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAileronAngle, float, Value);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHelicopterPhysicsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELICOPTERS_API IHelicopterPhysicsInterface
{
	GENERATED_BODY()

public:
	/** Called to set the aileron angle */
	virtual FNotifyCurrentAileronAngle& GetNotifyCurrentAileronAngle() = 0;
};
