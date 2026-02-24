// P6.2 Auto-Hover: Optional collective offset (e.g. altitude-hold) summed with pilot collective by Control.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HelicopterCollectiveOffsetProviderInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UHelicopterCollectiveOffsetProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class HELICOPTERS_API IHelicopterCollectiveOffsetProviderInterface
{
	GENERATED_BODY()

public:
	/** Called by Control with current pilot collective before GetCollectiveOffset (for override detection). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Helicopter|Collective")
	void NotifyPilotCollective(float PilotCollective);
	virtual void NotifyPilotCollective_Implementation(float PilotCollective) { (void)PilotCollective; }

	/** Returns collective offset to add to pilot collective (e.g. auto-hover PID output). 0 when inactive or pilot override. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Helicopter|Collective")
	float GetCollectiveOffset() const;
	virtual float GetCollectiveOffset_Implementation() const { return 0.f; }
};
