// Output delegate interface for aircraft physics — broadcasts control surface angles + airspeed
// for visual animation, HUD, or telemetry consumers.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AircraftPhysicsInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentElevatorAngle, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAircraftAileronAngle, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentRudderAngle, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentFlapsAngle, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentAirspeed, float, Value);

UINTERFACE(MinimalAPI)
class UAircraftPhysicsInterface : public UInterface
{
	GENERATED_BODY()
};

class AIRCRAFTS_API IAircraftPhysicsInterface
{
	GENERATED_BODY()

public:
	virtual FNotifyCurrentElevatorAngle& GetNotifyCurrentElevatorAngle() = 0;
	virtual FNotifyCurrentAircraftAileronAngle& GetNotifyCurrentAircraftAileronAngle() = 0;
	virtual FNotifyCurrentRudderAngle& GetNotifyCurrentRudderAngle() = 0;
	virtual FNotifyCurrentFlapsAngle& GetNotifyCurrentFlapsAngle() = 0;
	virtual FNotifyCurrentAirspeed& GetNotifyCurrentAirspeed() = 0;
};
