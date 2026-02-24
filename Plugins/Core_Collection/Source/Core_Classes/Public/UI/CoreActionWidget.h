// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CommonActionWidget.h"

#include "CoreActionWidget.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;

/**
 * UCoreActionWidget
 *
 * An action widget that will get the icon of key that is currently assigned to the common input action on this widget.
 * Adapted from ULyraActionWidget.
 */
UCLASS(BlueprintType, Blueprintable)
class CORE_CLASSES_API UCoreActionWidget : public UCommonActionWidget
{
	GENERATED_BODY()

public:

	//~ Begin UCommonActionWidget interface
	virtual FSlateBrush GetIcon() const override;
	//~ End of UCommonActionWidget interface

	/** The Enhanced Input Action that is associated with this Common Input action. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	const TObjectPtr<UInputAction> AssociatedInputAction;

private:

	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
	
};
