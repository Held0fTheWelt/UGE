// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CommonUserWidget.h"

#include "CoreReticleWidgetBase.generated.h"

class UObject;
struct FFrame;

/**
 * UCoreReticleWidgetBase
 *
 * Base widget for weapon reticle display.
 * Adapted from ULyraReticleWidgetBase.
 */
UCLASS(Abstract)
class CORE_CLASSES_API UCoreReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UCoreReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponInitialized();

	/** Initialize from a weapon object (GameFeature type); use IWeaponReticleInfoInterface for spread if implemented. */
	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(UObject* InWeapon);

	/** Returns the current weapon's diametrical spread angle, in degrees */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ComputeSpreadAngle() const;

	/** Returns the current weapon's maximum spread radius in screenspace units (pixels) */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ComputeMaxScreenspaceSpreadRadius() const;

	/**
	 * Returns true if the current weapon is at 'first shot accuracy'
	 * (the weapon allows it and it is at min spread)
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasFirstShotAccuracy() const;

protected:
	/** Current weapon instance (UObject; may implement Elemental interfaces). */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WeaponInstance;

	/** Associated inventory item instance if available (UObject; may implement IInventoryItemInstanceInterface). */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> InventoryInstance;
};
