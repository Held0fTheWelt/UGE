// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#pragma once

#include "DataSource/GameSettingDataSourceDynamic.h"
#include "GameSettingRegistry.h"
#include "Settings/CoreSettingsLocal.h"

#include "CoreGameSettingRegistry.generated.h"

class ULocalPlayer;
class UObject;

//--------------------------------------
// UCoreGameSettingRegistry
//--------------------------------------

class UGameSettingCollection;
class ULocalPlayer;

DECLARE_LOG_CATEGORY_EXTERN(LogCoreGameSettingRegistry, Log, Log);

#define GET_SHARED_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		TEXT("GetSharedSettings"),															\
		GET_FUNCTION_NAME_STRING_CHECKED(UCoreSettingsShared, FunctionOrPropertyName)		\
	}))

#define GET_LOCAL_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		TEXT("GetLocalSettings"),															\
		GET_FUNCTION_NAME_STRING_CHECKED(UCoreSettingsLocal, FunctionOrPropertyName)		\
	}))

/**
 * UCoreGameSettingRegistry
 *
 *	Game Setting Registry for Core.
 *	Adapted from LyraGameSettingRegistry.
 */
UCLASS()
class CORE_SETUP_API UCoreGameSettingRegistry : public UGameSettingRegistry
{
	GENERATED_BODY()

public:
	UCoreGameSettingRegistry();

	static UCoreGameSettingRegistry* Get(ULocalPlayer* InLocalPlayer);
	
	virtual void SaveChanges() override;

protected:
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) override;
	virtual bool IsFinishedInitializing() const override;

	UGameSettingCollection* InitializeVideoSettings(ULocalPlayer* InLocalPlayer);
	void InitializeVideoSettings_FrameRates(UGameSettingCollection* Screen, ULocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeAudioSettings(ULocalPlayer* InLocalPlayer);
	UGameSettingCollection* InitializeGameplaySettings(ULocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeMouseAndKeyboardSettings(ULocalPlayer* InLocalPlayer);
	UGameSettingCollection* InitializeGamepadSettings(ULocalPlayer* InLocalPlayer);

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> VideoSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> AudioSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GameplaySettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> MouseAndKeyboardSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GamepadSettings;
};
