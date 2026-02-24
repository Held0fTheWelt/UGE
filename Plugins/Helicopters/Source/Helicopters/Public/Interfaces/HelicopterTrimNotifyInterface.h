// P6.3 Auto-Trim: Notify when stick is released so trim target can be captured.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HelicopterTrimNotifyInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UHelicopterTrimNotifyInterface : public UInterface
{
	GENERATED_BODY()
};

class HELICOPTERS_API IHelicopterTrimNotifyInterface
{
	GENERATED_BODY()

public:
	/** Called when cyclic stick is released (e.g. to capture current attitude as trim). P6.3 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Helicopter|Trim")
	void NotifyStickReleasedForTrim();
	virtual void NotifyStickReleasedForTrim_Implementation() {}
};
