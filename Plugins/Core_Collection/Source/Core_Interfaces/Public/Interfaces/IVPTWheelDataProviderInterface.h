// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IVPTWheelDataProviderInterface.generated.h"

/** Per-wheel snapshot data for VPT alignment and suspension inspection */
USTRUCT(BlueprintType)
struct CORE_INTERFACES_API FVPTWheelSnapshot
{
	GENERATED_BODY()

	/** World-space position of the wheel center */
	UPROPERTY(BlueprintReadOnly, Category = "VPT|Wheel")
	FVector Position = FVector::ZeroVector;

	/** World-space contact point (if bContactValid is true) */
	UPROPERTY(BlueprintReadOnly, Category = "VPT|Wheel")
	FVector ContactPoint = FVector::ZeroVector;

	/** Toe angle in degrees (positive = toe-in) */
	UPROPERTY(BlueprintReadOnly, Category = "VPT|Wheel")
	float ToeAngle = 0.f;

	/** Camber angle in degrees (positive = top leaning outward) */
	UPROPERTY(BlueprintReadOnly, Category = "VPT|Wheel")
	float CamberAngle = 0.f;

	/** Current suspension travel (0 = fully extended, 1 = fully compressed) */
	UPROPERTY(BlueprintReadOnly, Category = "VPT|Wheel")
	float SuspensionTravel = 0.f;

	/** Whether the wheel is in contact with the ground */
	UPROPERTY(BlueprintReadOnly, Category = "VPT|Wheel")
	bool bContactValid = false;
};

UINTERFACE(MinimalAPI, BlueprintType)
class UVPTWheelDataProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Implemented by actors/components that expose per-wheel data for VPT inspection.
 * Enables vpt.Inspect.Wheels and vpt.Inspect.Alignment console commands.
 */
class CORE_INTERFACES_API IVPTWheelDataProviderInterface
{
	GENERATED_BODY()

public:
	/** Returns per-wheel snapshots for all wheels on this vehicle */
	virtual TArray<FVPTWheelSnapshot> GetVPTWheelSnapshots() const { return {}; }
};

static IVPTWheelDataProviderInterface* GetWheelProvider(AActor* Actor)
{
	if (!Actor) return nullptr;
	if (IVPTWheelDataProviderInterface* I = Cast<IVPTWheelDataProviderInterface>(Actor)) return I;
	TArray<UActorComponent*> Comps = Actor->GetComponentsByInterface(UVPTWheelDataProviderInterface::StaticClass());
	return Comps.Num() > 0 ? Cast<IVPTWheelDataProviderInterface>(Comps[0]) : nullptr;
}
