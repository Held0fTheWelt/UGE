// Copyright Epic Games, Inc. All Rights Reserved.
// Plan Paket 2: Interface für Keyboard-Input-Setting, damit Core_Classes nicht von Core_Setup abhängt.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "UObject/Interface.h"

#include "KeyboardInputSettingInterface.generated.h"

/**
 * Interface for a game setting that represents a keyboard/input binding (e.g. primary/secondary key).
 * Implemented by Core_Setup's UCoreSettingKeyboardInput.
 * Core_Classes (CoreSettingsListEntrySetting_KeyboardInput) uses only this interface.
 */
UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UKeyboardInputSettingInterface : public UInterface
{
	GENERATED_BODY()
};

class CORE_INTERFACES_API IKeyboardInputSettingInterface
{
	GENERATED_BODY()

public:
	/** Slot index: 0 = primary/first, 1 = secondary/second. */
	virtual FText GetKeyTextFromSlot(int32 SlotIndex) const { return FText::GetEmpty(); }
	virtual bool ChangeBinding(int32 SlotIndex, FKey NewKey) { return false; }
	virtual void GetAllMappedActionsFromKey(int32 SlotIndex, FKey Key, TArray<FName>& OutActionNames) const {}
	virtual bool IsMappingCustomized() const { return false; }
	/** Resets the key binding to default (same as UGameSettingValue::ResetToDefault). */
	virtual void ResetToDefault() {}
};
