// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CommonUserWidget.h"

#include "CoreWeaponUserInterface.generated.h"

class UObject;
struct FGeometry;

/**
 * UCoreWeaponUserInterface
 *
 * Base widget for weapon UI.
 * Adapted from ULyraWeaponUserInterface.
 */
UCLASS()
class CORE_CLASSES_API UCoreWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UCoreWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Called when the current weapon object changes. OldWeapon/NewWeapon are UObject* (GameFeature weapon instance). */
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(UObject* OldWeapon, UObject* NewWeapon);

	/** Set the current weapon from outside (e.g. from GameFeature or HUD); passes through IEquipmentManagerInterface if needed. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetCurrentWeapon(UObject* InWeapon);

private:
	void RebuildWidgetFromWeapon();

private:
	UPROPERTY(Transient)
	TObjectPtr<UObject> CurrentInstance;
};
