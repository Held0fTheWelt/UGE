// Auto-Trim: Notify when stick is released so trim target can be captured.
// Mirrors IHelicopterTrimNotifyInterface for fixed-wing aircraft.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AircraftTrimNotifyInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UAircraftTrimNotifyInterface : public UInterface
{
	GENERATED_BODY()
};

class AIRCRAFTS_API IAircraftTrimNotifyInterface
{
	GENERATED_BODY()

public:
	/** Called when control stick is released (e.g. to capture current attitude as trim). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Aircraft|Trim")
	void NotifyStickReleasedForTrim();
	virtual void NotifyStickReleasedForTrim_Implementation() {}
};
