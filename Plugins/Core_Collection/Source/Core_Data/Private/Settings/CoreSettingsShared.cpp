// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core - Settings in Core_Data

#include "Settings/CoreSettingsShared.h"

#include "Framework/Application/SlateApplication.h"
#include "Internationalization/Culture.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/App.h"
#include "Misc/ConfigCacheIni.h"
#include "Rendering/SlateRenderer.h"
#include "SubtitleDisplaySubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Engine/LocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreSettingsShared)

static FString SHARED_SETTINGS_SLOT_NAME = TEXT("SharedGameSettings");

namespace CoreSettingsSharedCVars
{
	static float DefaultGamepadLeftStickInnerDeadZone = 0.25f;
	static FAutoConsoleVariableRef CVarGamepadLeftStickInnerDeadZone(
		TEXT("gpad.DefaultLeftStickInnerDeadZone"),
		DefaultGamepadLeftStickInnerDeadZone,
		TEXT("Gamepad left stick inner deadzone")
	);

	static float DefaultGamepadRightStickInnerDeadZone = 0.25f;
	static FAutoConsoleVariableRef CVarGamepadRightStickInnerDeadZone(
		TEXT("gpad.DefaultRightStickInnerDeadZone"),
		DefaultGamepadRightStickInnerDeadZone,
		TEXT("Gamepad right stick inner deadzone")
	);	
}

UCoreSettingsShared::UCoreSettingsShared()
{
	FInternationalization::Get().OnCultureChanged().AddUObject(this, &ThisClass::OnCultureChanged);

	GamepadMoveStickDeadZone = CoreSettingsSharedCVars::DefaultGamepadLeftStickInnerDeadZone;
	GamepadLookStickDeadZone = CoreSettingsSharedCVars::DefaultGamepadRightStickInnerDeadZone;
}

void UCoreSettingsShared::AddSettingChangedListener(UObject* Listener, FName FunctionName)
{
	if (Listener && !FunctionName.IsNone())
	{
		FSettingChangedListenerEntry Entry;
		Entry.Listener = Listener;
		Entry.FunctionName = FunctionName;
		if (!SettingChangedListeners.ContainsByPredicate([&Entry](const FSettingChangedListenerEntry& E) { return E.Listener == Entry.Listener && E.FunctionName == Entry.FunctionName; }))
		{
			SettingChangedListeners.Add(Entry);
		}
	}
}

void UCoreSettingsShared::NotifySettingChangedListeners()
{
	for (int32 i = SettingChangedListeners.Num() - 1; i >= 0; --i)
	{
		UObject* Listener = SettingChangedListeners[i].Listener.Get();
		if (!Listener)
		{
			SettingChangedListeners.RemoveAt(i);
			continue;
		}
		if (UFunction* Func = Listener->FindFunction(SettingChangedListeners[i].FunctionName))
		{
			struct FOnSettingsChangedParams { UObject* Settings; };
			FOnSettingsChangedParams Params;
			Params.Settings = this;
			Listener->ProcessEvent(Func, &Params);
		}
	}
}

int32 UCoreSettingsShared::GetLatestDataVersion() const
{
	return 1;
}

UCoreSettingsShared* UCoreSettingsShared::CreateTemporarySettings(const ULocalPlayer* LocalPlayer)
{
	UCoreSettingsShared* SharedSettings = Cast<UCoreSettingsShared>(CreateNewSaveGameForLocalPlayer(UCoreSettingsShared::StaticClass(), LocalPlayer, SHARED_SETTINGS_SLOT_NAME));
	SharedSettings->ApplySettings();
	return SharedSettings;
}

UCoreSettingsShared* UCoreSettingsShared::LoadOrCreateSettings(const ULocalPlayer* LocalPlayer)
{
	UCoreSettingsShared* SharedSettings = Cast<UCoreSettingsShared>(LoadOrCreateSaveGameForLocalPlayer(UCoreSettingsShared::StaticClass(), LocalPlayer, SHARED_SETTINGS_SLOT_NAME));
	SharedSettings->ApplySettings();
	return SharedSettings;
}

bool UCoreSettingsShared::AsyncLoadOrCreateSettings(const ULocalPlayer* LocalPlayer, FOnSettingsLoadedEvent Delegate)
{
	FOnLocalPlayerSaveGameLoadedNative Lambda = FOnLocalPlayerSaveGameLoadedNative::CreateLambda([Delegate](ULocalPlayerSaveGame* LoadedSave)
		{
			UCoreSettingsShared* LoadedSettings = CastChecked<UCoreSettingsShared>(LoadedSave);
			LoadedSettings->ApplySettings();
			Delegate.ExecuteIfBound(LoadedSettings);
		});
	return ULocalPlayerSaveGame::AsyncLoadOrCreateSaveGameForLocalPlayer(UCoreSettingsShared::StaticClass(), LocalPlayer, SHARED_SETTINGS_SLOT_NAME, Lambda);
}

void UCoreSettingsShared::SaveSettings()
{
	AsyncSaveGameToSlotForLocalPlayer();

	if (UEnhancedInputLocalPlayerSubsystem* System = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwningPlayer))
	{
		if (UEnhancedInputUserSettings* InputSettings = System->GetUserSettings())
		{
			InputSettings->AsyncSaveSettings();
		}
	}
}

void UCoreSettingsShared::ApplySettings()
{
	ApplySubtitleOptions();
	ApplyBackgroundAudioSettings();
	ApplyInputSensitivity();
	ApplyCultureSettings();
}

ECoreColorBlindMode UCoreSettingsShared::GetColorBlindMode() const
{
	return ColorBlindMode;
}

void UCoreSettingsShared::SetColorBlindMode(ECoreColorBlindMode InMode)
{
	if (ChangeValueAndDirty(ColorBlindMode, InMode))
	{
		FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(
			(EColorVisionDeficiency)(int32)ColorBlindMode, (int32)ColorBlindStrength, true, false);
	}
}

int32 UCoreSettingsShared::GetColorBlindStrength() const
{
	return ColorBlindStrength;
}

void UCoreSettingsShared::SetColorBlindStrength(int32 InColorBlindStrength)
{
	InColorBlindStrength = FMath::Clamp(InColorBlindStrength, 0, 10);
	if (ChangeValueAndDirty(ColorBlindStrength, InColorBlindStrength))
	{
		FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(
			(EColorVisionDeficiency)(int32)ColorBlindMode, (int32)ColorBlindStrength, true, false);
	}
}

void UCoreSettingsShared::ApplySubtitleOptions()
{
	if (USubtitleDisplaySubsystem* SubtitleSystem = USubtitleDisplaySubsystem::Get(OwningPlayer))
	{
		FSubtitleFormat SubtitleFormat;
		SubtitleFormat.SubtitleTextSize = SubtitleTextSize;
		SubtitleFormat.SubtitleTextColor = SubtitleTextColor;
		SubtitleFormat.SubtitleTextBorder = SubtitleTextBorder;
		SubtitleFormat.SubtitleBackgroundOpacity = SubtitleBackgroundOpacity;

		SubtitleSystem->SetSubtitleDisplayOptions(SubtitleFormat);
	}
}

void UCoreSettingsShared::SetAllowAudioInBackgroundSetting(ECoreAllowBackgroundAudioSetting NewValue)
{
	ChangeValueAndDirty(AllowAudioInBackground, NewValue);
	ApplyBackgroundAudioSettings();
}

void UCoreSettingsShared::ApplyBackgroundAudioSettings()
{
	if (OwningPlayer && OwningPlayer->IsPrimaryPlayer())
	{
		FApp::SetUnfocusedVolumeMultiplier((AllowAudioInBackground != ECoreAllowBackgroundAudioSetting::Off) ? 1.0f : 0.0f);
	}
}

const FString& UCoreSettingsShared::GetPendingCulture() const
{
	return PendingCulture;
}

void UCoreSettingsShared::SetPendingCulture(const FString& NewCulture)
{
	PendingCulture = NewCulture;
	bResetToDefaultCulture = false;
	bIsDirty = true;
}

void UCoreSettingsShared::OnCultureChanged()
{
	ClearPendingCulture();
	bResetToDefaultCulture = false;
}

void UCoreSettingsShared::ClearPendingCulture()
{
	PendingCulture.Empty();
}

bool UCoreSettingsShared::IsUsingDefaultCulture() const
{
	FString Culture;
	GConfig->GetString(TEXT("Internationalization"), TEXT("Culture"), Culture, GGameUserSettingsIni);
	return Culture.IsEmpty();
}

void UCoreSettingsShared::ResetToDefaultCulture()
{
	ClearPendingCulture();
	bResetToDefaultCulture = true;
	bIsDirty = true;
}

void UCoreSettingsShared::ApplyCultureSettings()
{
	if (bResetToDefaultCulture)
	{
		const FCulturePtr SystemDefaultCulture = FInternationalization::Get().GetDefaultCulture();
		check(SystemDefaultCulture.IsValid());

		const FString CultureToApply = SystemDefaultCulture->GetName();
		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
		{
			GConfig->RemoveKey(TEXT("Internationalization"), TEXT("Culture"), GGameUserSettingsIni);
			GConfig->Flush(false, GGameUserSettingsIni);
		}
		bResetToDefaultCulture = false;
	}
	else if (!PendingCulture.IsEmpty())
	{
		const FString CultureToApply = PendingCulture;
		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
		{
			GConfig->SetString(TEXT("Internationalization"), TEXT("Culture"), *CultureToApply, GGameUserSettingsIni);
			GConfig->Flush(false, GGameUserSettingsIni);
		}
		ClearPendingCulture();
	}
}

void UCoreSettingsShared::ResetCultureToCurrentSettings()
{
	ClearPendingCulture();
	bResetToDefaultCulture = false;
}

void UCoreSettingsShared::ApplyInputSensitivity()
{
	// Apply to Enhanced Input when available; gamepad presets read by game
}

void UCoreSettingsShared::SetGamepadMoveStickDeadZone(const float NewValue)
{
	ChangeValueAndDirty(GamepadMoveStickDeadZone, NewValue);
}

void UCoreSettingsShared::SetGamepadLookStickDeadZone(const float NewValue)
{
	ChangeValueAndDirty(GamepadLookStickDeadZone, NewValue);
}

void UCoreSettingsShared::SetTriggerHapticsEnabled(const bool NewValue)
{
	ChangeValueAndDirty(bTriggerHapticsEnabled, NewValue);
}

void UCoreSettingsShared::SetTriggerPullUsesHapticThreshold(const bool NewValue)
{
	ChangeValueAndDirty(bTriggerPullUsesHapticThreshold, NewValue);
}

void UCoreSettingsShared::SetTriggerHapticStrength(const uint8 NewValue)
{
	ChangeValueAndDirty(TriggerHapticStrength, NewValue);
}

void UCoreSettingsShared::SetTriggerHapticStartPosition(const uint8 NewValue)
{
	ChangeValueAndDirty(TriggerHapticStartPosition, NewValue);
}

void UCoreSettingsShared::SetSubtitlesEnabled(bool Value)
{
	ChangeValueAndDirty(bEnableSubtitles, Value);
}

void UCoreSettingsShared::SetSubtitlesTextSize(ESubtitleDisplayTextSize Value)
{
	ChangeValueAndDirty(SubtitleTextSize, Value);
	ApplySubtitleOptions();
}

void UCoreSettingsShared::SetSubtitlesTextColor(ESubtitleDisplayTextColor Value)
{
	ChangeValueAndDirty(SubtitleTextColor, Value);
	ApplySubtitleOptions();
}

void UCoreSettingsShared::SetSubtitlesTextBorder(ESubtitleDisplayTextBorder Value)
{
	ChangeValueAndDirty(SubtitleTextBorder, Value);
	ApplySubtitleOptions();
}

void UCoreSettingsShared::SetSubtitlesBackgroundOpacity(ESubtitleDisplayBackgroundOpacity Value)
{
	ChangeValueAndDirty(SubtitleBackgroundOpacity, Value);
	ApplySubtitleOptions();
}

void UCoreSettingsShared::SetMouseSensitivityX(double NewValue)
{
	ChangeValueAndDirty(MouseSensitivityX, NewValue);
	ApplyInputSensitivity();
}

void UCoreSettingsShared::SetMouseSensitivityY(double NewValue)
{
	ChangeValueAndDirty(MouseSensitivityY, NewValue);
	ApplyInputSensitivity();
}

void UCoreSettingsShared::SetTargetingMultiplier(double NewValue)
{
	ChangeValueAndDirty(TargetingMultiplier, NewValue);
	ApplyInputSensitivity();
}

void UCoreSettingsShared::SetInvertVerticalAxis(bool NewValue)
{
	ChangeValueAndDirty(bInvertVerticalAxis, NewValue);
	ApplyInputSensitivity();
}

void UCoreSettingsShared::SetInvertHorizontalAxis(bool NewValue)
{
	ChangeValueAndDirty(bInvertHorizontalAxis, NewValue);
	ApplyInputSensitivity();
}

void UCoreSettingsShared::SetLookSensitivityPreset(ECoreGamepadSensitivity NewValue)
{
	ChangeValueAndDirty(GamepadLookSensitivityPreset, NewValue);
	ApplyInputSensitivity();
}

void UCoreSettingsShared::SetGamepadTargetingSensitivityPreset(ECoreGamepadSensitivity NewValue)
{
	ChangeValueAndDirty(GamepadTargetingSensitivityPreset, NewValue);
	ApplyInputSensitivity();
}
