// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Input/CommonBoundActionButton.h"

#include "CoreBoundActionButton.generated.h"

class UCommonButtonStyle;
class UObject;

/**
 * UCoreBoundActionButton
 *
 * Bound action button with input method style support.
 * Adapted from ULyraBoundActionButton.
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class CORE_CLASSES_API UCoreBoundActionButton : public UCommonBoundActionButton
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	void HandleInputMethodChanged(ECommonInputType NewInputMethod);

	UPROPERTY(EditAnywhere, Category = "Styles")
	TSubclassOf<UCommonButtonStyle> KeyboardStyle;

	UPROPERTY(EditAnywhere, Category = "Styles")
	TSubclassOf<UCommonButtonStyle> GamepadStyle;

	UPROPERTY(EditAnywhere, Category = "Styles")
	TSubclassOf<UCommonButtonStyle> TouchStyle;
};
