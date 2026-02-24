// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core - Settings types in Core_Data

#pragma once

#include "GameFramework/SaveGame.h"
#include "SubtitleDisplayOptions.h"

#include "UObject/ObjectPtr.h"
#include "Interfaces/SharedSettingsForceFeedbackInterface.h"
#include "CoreSettingsShared.generated.h"

class UObject;
struct FFrame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoreSettingChangedDynamic, UObject*, Settings);

USTRUCT()
struct FSettingChangedListenerEntry
{
	GENERATED_BODY()
	UPROPERTY(Transient)
	TWeakObjectPtr<UObject> Listener;
	UPROPERTY(Transient)
	FName FunctionName;
};

UENUM(BlueprintType)
enum class ECoreColorBlindMode : uint8
{
	Off,
	Deuteranope,
	Protanope,
	Tritanope
};

UENUM(BlueprintType)
enum class ECoreAllowBackgroundAudioSetting : uint8
{
	Off,
	AllSounds,

	Num UMETA(Hidden),
};

UENUM(BlueprintType)
enum class ECoreGamepadSensitivity : uint8
{
	Invalid = 0		UMETA(Hidden),

	Slow			UMETA(DisplayName = "01 - Slow"),
	SlowPlus		UMETA(DisplayName = "02 - Slow+"),
	SlowPlusPlus	UMETA(DisplayName = "03 - Slow++"),
	Normal			UMETA(DisplayName = "04 - Normal"),
	NormalPlus		UMETA(DisplayName = "05 - Normal+"),
	NormalPlusPlus	UMETA(DisplayName = "06 - Normal++"),
	Fast			UMETA(DisplayName = "07 - Fast"),
	FastPlus		UMETA(DisplayName = "08 - Fast+"),
	FastPlusPlus	UMETA(DisplayName = "09 - Fast++"),
	Insane			UMETA(DisplayName = "10 - Insane"),

	MAX				UMETA(Hidden),
};

class ULocalPlayer;

/**
 * UCoreSettingsShared - Shared settings stored via USaveGame system.
 * Adapted from LyraSettingsShared.
 */
UCLASS()
class CORE_DATA_API UCoreSettingsShared : public ULocalPlayerSaveGame, public ISharedSettingsForceFeedbackInterface
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(UCoreSettingsShared, FOnSettingChangedEvent, UCoreSettingsShared* Settings);
	FOnSettingChangedEvent OnSettingChanged;

	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FOnCoreSettingChangedDynamic OnSettingChangedDynamic;

public:

	UCoreSettingsShared();

	//~ULocalPlayerSaveGame interface
	int32 GetLatestDataVersion() const override;
	//~End of ULocalPlayerSaveGame interface

	bool IsDirty() const { return bIsDirty; }
	void ClearDirtyFlag() { bIsDirty = false; }

	static UCoreSettingsShared* CreateTemporarySettings(const ULocalPlayer* LocalPlayer);
	static UCoreSettingsShared* LoadOrCreateSettings(const ULocalPlayer* LocalPlayer);

	DECLARE_DELEGATE_OneParam(FOnSettingsLoadedEvent, UCoreSettingsShared* Settings);
	static bool AsyncLoadOrCreateSettings(const ULocalPlayer* LocalPlayer, FOnSettingsLoadedEvent Delegate);

	void SaveSettings();
	void ApplySettings();
	
public:
	// Color Blind Options
	UFUNCTION()
	ECoreColorBlindMode GetColorBlindMode() const;
	UFUNCTION()
	void SetColorBlindMode(ECoreColorBlindMode InMode);
	UFUNCTION()
	int32 GetColorBlindStrength() const;
	UFUNCTION()
	void SetColorBlindStrength(int32 InColorBlindStrength);

private:
	UPROPERTY()
	ECoreColorBlindMode ColorBlindMode = ECoreColorBlindMode::Off;
	UPROPERTY()
	int32 ColorBlindStrength = 10;

	// Gamepad Vibration
public:
	UFUNCTION()
	bool GetForceFeedbackEnabled() const override { return bForceFeedbackEnabled; }
	UFUNCTION()
	void AddSettingChangedListener(UObject* Listener, FName FunctionName) override;
	UFUNCTION()
	void SetForceFeedbackEnabled(const bool NewValue) { ChangeValueAndDirty(bForceFeedbackEnabled, NewValue); }
	
private:
	UPROPERTY()
	bool bForceFeedbackEnabled = true;

	// Gamepad Deadzone
public:
	UFUNCTION()
	float GetGamepadMoveStickDeadZone() const { return GamepadMoveStickDeadZone; }
	UFUNCTION()
	void SetGamepadMoveStickDeadZone(const float NewValue);
	UFUNCTION()
	float GetGamepadLookStickDeadZone() const { return GamepadLookStickDeadZone; }
	UFUNCTION()
	void SetGamepadLookStickDeadZone(const float NewValue);

private:
	UPROPERTY()
	float GamepadMoveStickDeadZone;
	UPROPERTY()
	float GamepadLookStickDeadZone;

	// Gamepad Trigger Haptics
public:
	UFUNCTION()
	bool GetTriggerHapticsEnabled() const { return bTriggerHapticsEnabled; }
	UFUNCTION()
	void SetTriggerHapticsEnabled(const bool NewValue);
	UFUNCTION()
	bool GetTriggerPullUsesHapticThreshold() const { return bTriggerPullUsesHapticThreshold; }
	UFUNCTION()
	void SetTriggerPullUsesHapticThreshold(const bool NewValue);
	UFUNCTION()
	uint8 GetTriggerHapticStrength() const { return TriggerHapticStrength; }
	UFUNCTION()
	void SetTriggerHapticStrength(const uint8 NewValue);
	UFUNCTION()
	uint8 GetTriggerHapticStartPosition() const { return TriggerHapticStartPosition; }
	UFUNCTION()
	void SetTriggerHapticStartPosition(const uint8 NewValue);
	
private:
	UPROPERTY()
	bool bTriggerHapticsEnabled = false;
	UPROPERTY()
	bool bTriggerPullUsesHapticThreshold = true;
	UPROPERTY()
	uint8 TriggerHapticStrength = 8;
	UPROPERTY()
	uint8 TriggerHapticStartPosition = 0;

	// Subtitles
public:
	UFUNCTION()
	bool GetSubtitlesEnabled() const { return bEnableSubtitles; }
	UFUNCTION()
	void SetSubtitlesEnabled(bool Value);
	UFUNCTION()
	ESubtitleDisplayTextSize GetSubtitlesTextSize() const { return SubtitleTextSize; }
	UFUNCTION()
	void SetSubtitlesTextSize(ESubtitleDisplayTextSize Value);
	UFUNCTION()
	ESubtitleDisplayTextColor GetSubtitlesTextColor() const { return SubtitleTextColor; }
	UFUNCTION()
	void SetSubtitlesTextColor(ESubtitleDisplayTextColor Value);
	UFUNCTION()
	ESubtitleDisplayTextBorder GetSubtitlesTextBorder() const { return SubtitleTextBorder; }
	UFUNCTION()
	void SetSubtitlesTextBorder(ESubtitleDisplayTextBorder Value);
	UFUNCTION()
	ESubtitleDisplayBackgroundOpacity GetSubtitlesBackgroundOpacity() const { return SubtitleBackgroundOpacity; }
	UFUNCTION()
	void SetSubtitlesBackgroundOpacity(ESubtitleDisplayBackgroundOpacity Value);
	void ApplySubtitleOptions();

private:
	UPROPERTY()
	bool bEnableSubtitles = true;
	UPROPERTY()
	ESubtitleDisplayTextSize SubtitleTextSize = ESubtitleDisplayTextSize::Medium;
	UPROPERTY()
	ESubtitleDisplayTextColor SubtitleTextColor = ESubtitleDisplayTextColor::White;
	UPROPERTY()
	ESubtitleDisplayTextBorder SubtitleTextBorder = ESubtitleDisplayTextBorder::None;
	UPROPERTY()
	ESubtitleDisplayBackgroundOpacity SubtitleBackgroundOpacity = ESubtitleDisplayBackgroundOpacity::Medium;

	// Shared audio settings
public:
	UFUNCTION()
	ECoreAllowBackgroundAudioSetting GetAllowAudioInBackgroundSetting() const { return AllowAudioInBackground; }
	UFUNCTION()
	void SetAllowAudioInBackgroundSetting(ECoreAllowBackgroundAudioSetting NewValue);
	void ApplyBackgroundAudioSettings();

private:
	UPROPERTY()
	ECoreAllowBackgroundAudioSetting AllowAudioInBackground = ECoreAllowBackgroundAudioSetting::Off;

	// Culture / language
public:
	const FString& GetPendingCulture() const;
	void SetPendingCulture(const FString& NewCulture);
	void OnCultureChanged();
	void ClearPendingCulture();
	bool IsUsingDefaultCulture() const;
	void ResetToDefaultCulture();
	bool ShouldResetToDefaultCulture() const { return bResetToDefaultCulture; }
	void ApplyCultureSettings();
	void ResetCultureToCurrentSettings();

private:
	UPROPERTY(Transient)
	FString PendingCulture;
	bool bResetToDefaultCulture = false;

	// Mouse Sensitivity
public:
	UFUNCTION()
	double GetMouseSensitivityX() const { return MouseSensitivityX; }
	UFUNCTION()
	void SetMouseSensitivityX(double NewValue);
	UFUNCTION()
	double GetMouseSensitivityY() const { return MouseSensitivityY; }
	UFUNCTION()
	void SetMouseSensitivityY(double NewValue);
	UFUNCTION()
	double GetTargetingMultiplier() const { return TargetingMultiplier; }
	UFUNCTION()
	void SetTargetingMultiplier(double NewValue);
	UFUNCTION()
	bool GetInvertVerticalAxis() const { return bInvertVerticalAxis; }
	UFUNCTION()
	void SetInvertVerticalAxis(bool NewValue);
	UFUNCTION()
	bool GetInvertHorizontalAxis() const { return bInvertHorizontalAxis; }
	UFUNCTION()
	void SetInvertHorizontalAxis(bool NewValue);
	
private:
	UPROPERTY()
	double MouseSensitivityX = 1.0;
	UPROPERTY()
	double MouseSensitivityY = 1.0;
	UPROPERTY()
	double TargetingMultiplier = 0.5;
	UPROPERTY()
	bool bInvertVerticalAxis = false;
	UPROPERTY()
	bool bInvertHorizontalAxis = false;
	
	// Gamepad Sensitivity
public:
	UFUNCTION()
	ECoreGamepadSensitivity GetGamepadLookSensitivityPreset() const { return GamepadLookSensitivityPreset; }
	UFUNCTION()
	void SetLookSensitivityPreset(ECoreGamepadSensitivity NewValue);
	UFUNCTION()
	ECoreGamepadSensitivity GetGamepadTargetingSensitivityPreset() const { return GamepadTargetingSensitivityPreset; }
	UFUNCTION()
	void SetGamepadTargetingSensitivityPreset(ECoreGamepadSensitivity NewValue);
	void ApplyInputSensitivity();
	
private:
	UPROPERTY()
	ECoreGamepadSensitivity GamepadLookSensitivityPreset = ECoreGamepadSensitivity::Normal;
	UPROPERTY()
	ECoreGamepadSensitivity GamepadTargetingSensitivityPreset = ECoreGamepadSensitivity::Normal;
	
private:
	void NotifySettingChangedListeners();

	template<typename T>
	bool ChangeValueAndDirty(T& CurrentValue, const T& NewValue)
	{
		if (CurrentValue != NewValue)
		{
			CurrentValue = NewValue;
			bIsDirty = true;
			OnSettingChanged.Broadcast(this);
			OnSettingChangedDynamic.Broadcast(this);
			NotifySettingChangedListeners();
			return true;
		}
		return false;
	}

	UPROPERTY(Transient)
	TArray<FSettingChangedListenerEntry> SettingChangedListeners;

	bool bIsDirty = false;
};
