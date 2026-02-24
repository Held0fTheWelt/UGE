// Copyright Epic Games, Inc. All Rights Reserved.
// Plan Paket 4: Abstrakte Basis für Performance-Stat-Subsystem; Core_Classes nutzt nur diese API.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PerformanceStats/PerformanceStatTypes.h"

#include "PerformanceStatSubsystemBase.generated.h"

/**
 * Base class for the performance stat subsystem. Implemented by Core_Performance's UCorePerformanceStatSubsystem.
 * Core_Classes uses only this base and the enums from Core_Interfaces, so it does not depend on Core_Performance.
 */
UCLASS(Abstract)
class CORE_INTERFACES_API UPerformanceStatSubsystemBase : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Returns the current cached value for the given stat. */
	UFUNCTION(BlueprintCallable, Category = "Performance")
	virtual double GetCachedStat(ECoreDisplayablePerformanceStat Stat) const { return 0.0; }

	/** Fills OutSamples with the current sample ring buffer for the given stat (same order as display). */
	virtual void GetCachedStatSamples(ECoreDisplayablePerformanceStat Stat, TArray<double>& OutSamples) const {}
};
