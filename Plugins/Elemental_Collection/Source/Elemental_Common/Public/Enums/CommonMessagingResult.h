#pragma once

#include "CommonMessagingResult.generated.h"

/** Possible results from a dialog */
UENUM(BlueprintType)
enum class ECommonMessagingResult : uint8
{
	/** The "yes" button was pressed */
	Confirmed,
	/** The "no" button was pressed */
	Declined,
	/** The "ignore/cancel" button was pressed */
	Cancelled,
	/** The dialog was explicitly killed (no user input) */
	Killed,
	Unknown UMETA(Hidden)
};