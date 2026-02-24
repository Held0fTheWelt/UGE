
#pragma once

#include "CoreMinimal.h"
#include "AutoHoverSettings.generated.h"

/**
 * FAutoHoverSettings
 * Structure representing the auto-hover settings for the helicopter.
 */
USTRUCT(BlueprintType)
struct HELICOPTERS_API FAutoHoverSettings
{
	GENERATED_BODY()

public:
	/** Default constructor */
	FAutoHoverSettings() : 
		bAutoHoveringEnabled(false),
		bMaintainAltitude(false),
		bMaintainVerticalSpeed(false),
		bMaintainLocation(false),
		bMaintainYaw(false),
		bMaintainHorizontalSpeed(false)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Hover")
	bool bAutoHoveringEnabled = false; // Whether auto-hovering is enabled or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Hover")
	bool bMaintainAltitude = false; // Whether to maintain altitude during auto-hovering
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Hover")
	bool bMaintainVerticalSpeed = false; // Whether to maintain vertical speed during auto-hovering
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Hover")
	bool bMaintainLocation = false; // Whether to maintain location during auto-hovering
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Hover")
	bool bMaintainYaw = false; // Whether to maintain yaw during auto-hovering
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Hover")
	bool bMaintainHorizontalSpeed = false; // Whether to maintain horizontal speed during auto-hovering

};