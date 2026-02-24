// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DataAssets/HeliFlightPresetDataAsset.h"
#include "HelicopterSASInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifySASStateChanged, bool, bEnabled, EHeliSASHoldMode, HoldMode);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHelicopterSASInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for Stability Augmented System (SAS) components.
 * Implemented by SASHelicopterComponent (GameFeature_StabilityAugmentedSystem).
 * Called by HelicopterControlComponent on SAS toggle key press.
 */
class HELICOPTERS_API IHelicopterSASInterface
{
	GENERATED_BODY()

public:
	/** Toggle SAS on/off (called by HelicopterControlComponent on key press). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SAS")
	void ToggleSAS();

	/** Query whether SAS is currently enabled. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SAS")
	bool IsSASEnabled() const;

	/** Query current SAS hold mode. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SAS")
	EHeliSASHoldMode GetSASHoldMode() const;
};
