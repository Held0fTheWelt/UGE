// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IVPTInspectableInterface.generated.h"

/** Supported feature flags for VPT inspection */
UENUM(BlueprintType)
enum class EVPTFeature : uint8
{
	Wheels      UMETA(DisplayName = "Wheels"),
	Alignment   UMETA(DisplayName = "Alignment"),
	Suspension  UMETA(DisplayName = "Suspension"),
	Telemetry   UMETA(DisplayName = "Telemetry"),
	Setup       UMETA(DisplayName = "Setup"),
	Tune        UMETA(DisplayName = "Tune"),
	Draw        UMETA(DisplayName = "Draw"),
};

UINTERFACE(MinimalAPI, BlueprintType)
class UVPTInspectableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Implemented by actors/components that can be targeted by the VPT inspection system.
 * Enables vpt.Target.* and vpt.Inspect.* console commands to identify and query the actor.
 */
class CORE_INTERFACES_API IVPTInspectableInterface
{
	GENERATED_BODY()

public:
	/** Display name shown in vpt.Target.Print / vpt.Inspect.PrintSummary */
	virtual FString GetVPTTargetName() const { return FString(); }

	/** Number of physics bodies (for vpt.Inspect.ListBodies) */
	virtual int32 GetVPTPhysicsBodyCount() const { return 0; }

	/** Check whether a specific VPT feature is supported by this actor */
	virtual bool SupportsVPTFeature(EVPTFeature Feature) const { return false; }
};
