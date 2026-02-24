// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ShipControlsInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentHelm, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentShipThrottle, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentBowThruster, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAnchor, bool, IsDeployed);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UShipControlsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class SHIPS_API IShipControlsInterface
{
	GENERATED_BODY()

public:
	virtual FNotifyCurrentHelm& GetNotifyCurrentHelm() = 0;
	virtual FNotifyCurrentShipThrottle& GetNotifyCurrentShipThrottle() = 0;
	virtual FNotifyCurrentBowThruster& GetNotifyCurrentBowThruster() = 0;
	virtual FNotifyCurrentAnchor& GetNotifyCurrentAnchor() = 0;
};
