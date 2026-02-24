// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "EnhancedActionKeyMapping.h"
#include "GameSettingValue.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Interfaces/KeyboardInputSettingInterface.h"

#include "CoreSettingKeyboardInput.generated.h"

class UObject;

//--------------------------------------
// UCoreSettingKeyboardInput
//--------------------------------------

UCLASS(meta = (Interfaces = "KeyboardInputSettingInterface"))
class CORE_SETUP_API UCoreSettingKeyboardInput : public UGameSettingValue, public IKeyboardInputSettingInterface
{
	GENERATED_BODY()

public:
	UCoreSettingKeyboardInput();

	void InitializeInputData(const UEnhancedPlayerMappableKeyProfile* KeyProfile, const FKeyMappingRow& MappingData, const FPlayerMappableKeyQueryOptions& QueryOptions);

	FText GetKeyTextFromSlot(const EPlayerMappableKeySlot InSlot) const;
	/** IKeyboardInputSettingInterface (SlotIndex: 0=First, 1=Second) */
	virtual FText GetKeyTextFromSlot(int32 SlotIndex) const override;
	
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	virtual bool ChangeBinding(int32 InKeyBindSlot, FKey NewKey) override;
	virtual void GetAllMappedActionsFromKey(int32 InKeyBindSlot, FKey Key, TArray<FName>& OutActionNames) const override;

	/** Returns true if mappings on this setting have been customized */
	virtual bool IsMappingCustomized() const override;

	FText GetSettingDisplayName() const;
	FText GetSettingDisplayCategory() const;

	const FKeyMappingRow* FindKeyMappingRow() const;
	UEnhancedPlayerMappableKeyProfile* FindMappableKeyProfile() const;
	UEnhancedInputUserSettings* GetUserSettings() const;
	
protected:
	/** UCoreSetting */
	virtual void OnInitialized() override;

protected:

	/** The name of this action's mappings */
	FName ActionMappingName;

	/** The query options to filter down keys on this setting for */
	FPlayerMappableKeyQueryOptions QueryOptions;

	/** The profile identifier that this key setting is from */
	FString ProfileIdentifier;

	/** Store the initial key mappings that are set on this for each slot */
	TMap<EPlayerMappableKeySlot, FKey> InitialKeyMappings;
};
