// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementalSettings_Shared.h"

// Hierarchy: Shared settings implementation lives in Core_Setup (UCoreSettingsShared).
// The block below is Lyra-origin commented-out code; adapt to Core types if re-enabling here.
//
//#include "Framework/Application/SlateApplication.h"
//#include "Internationalization/Culture.h"
//#include "Kismet/GameplayStatics.h"
//#include "Misc/App.h"
//#include "Misc/ConfigCacheIni.h"
//#include "Player/LyraLocalPlayer.h"
//#include "Rendering/SlateRenderer.h"
//#include "SubtitleDisplaySubsystem.h"
//#include "EnhancedInputSubsystems.h"
//#include "UserSettings/EnhancedInputUserSettings.h"
//
//#include UE_INLINE_GENERATED_CPP_BY_NAME(ElementalSettings_Shared)
//
//static FString SHARED_SETTINGS_SLOT_NAME = TEXT("SharedGameSettings");
//
//namespace LyraSettingsSharedCVars
//{
//	static float DefaultGamepadLeftStickInnerDeadZone = 0.25f;
//	static FAutoConsoleVariableRef CVarGamepadLeftStickInnerDeadZone(
//		TEXT("gpad.DefaultLeftStickInnerDeadZone"),
//		DefaultGamepadLeftStickInnerDeadZone,
//		TEXT("Gamepad left stick inner deadzone")
//	);
//
//	static float DefaultGamepadRightStickInnerDeadZone = 0.25f;
//	static FAutoConsoleVariableRef CVarGamepadRightStickInnerDeadZone(
//		TEXT("gpad.DefaultRightStickInnerDeadZone"),
//		DefaultGamepadRightStickInnerDeadZone,
//		TEXT("Gamepad right stick inner deadzone")
//	);
//}
//
//UElementalSettings_Shared::UElementalSettings_Shared()
//{
//	FInternationalization::Get().OnCultureChanged().AddUObject(this, &ThisClass::OnCultureChanged);
//
//	GamepadMoveStickDeadZone = LyraSettingsSharedCVars::DefaultGamepadLeftStickInnerDeadZone;
//	GamepadLookStickDeadZone = LyraSettingsSharedCVars::DefaultGamepadRightStickInnerDeadZone;
//}
//
//int32 UElementalSettings_Shared::GetLatestDataVersion() const
//{
//	// 0 = before subclassing ULocalPlayerSaveGame
//	// 1 = first proper version
//	return 1;
//}
//
//UElementalSettings_Shared* UElementalSettings_Shared::CreateTemporarySettings(const ULyraLocalPlayer* LocalPlayer)
//{
//	// This is not loaded from disk but should be set up to save
//	ULyraSettingsShared* SharedSettings = Cast<ULyraSettingsShared>(CreateNewSaveGameForLocalPlayer(ULyraSettingsShared::StaticClass(), LocalPlayer, SHARED_SETTINGS_SLOT_NAME));
//
//	SharedSettings->ApplySettings();
//
//	return SharedSettings;
//}
//
//UElementalSettings_Shared* UElementalSettings_Shared::LoadOrCreateSettings(const ULyraLocalPlayer* LocalPlayer)
//{
//	// This will stall the main thread while it loads
//	ULyraSettingsShared* SharedSettings = Cast<ULyraSettingsShared>(LoadOrCreateSaveGameForLocalPlayer(ULyraSettingsShared::StaticClass(), LocalPlayer, SHARED_SETTINGS_SLOT_NAME));
//
//	SharedSettings->ApplySettings();
//
//	return SharedSettings;
//}
//
//bool UElementalSettings_Shared::AsyncLoadOrCreateSettings(const ULyraLocalPlayer* LocalPlayer, FOnSettingsLoadedEvent Delegate)
//{
//	FOnLocalPlayerSaveGameLoadedNative Lambda = FOnLocalPlayerSaveGameLoadedNative::CreateLambda([Delegate]
//	(ULocalPlayerSaveGame* LoadedSave)
//		{
//			ULyraSettingsShared* LoadedSettings = CastChecked<ULyraSettingsShared>(LoadedSave);
//
//			LoadedSettings->ApplySettings();
//
//			Delegate.ExecuteIfBound(LoadedSettings);
//		});
//
//	return ULocalPlayerSaveGame::AsyncLoadOrCreateSaveGameForLocalPlayer(ULyraSettingsShared::StaticClass(), LocalPlayer, SHARED_SETTINGS_SLOT_NAME, Lambda);
//}
//
//void UElementalSettings_Shared::SaveSettings()
//{
//	// Schedule an async save because it's okay if it fails
//	AsyncSaveGameToSlotForLocalPlayer();
//
//	// TODO_BH: Move this to the serialize function instead with a bumped version number
//	if (UEnhancedInputLocalPlayerSubsystem* System = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwningPlayer))
//	{
//		if (UEnhancedInputUserSettings* InputSettings = System->GetUserSettings())
//		{
//			InputSettings->AsyncSaveSettings();
//		}
//	}
//}
//
//void UElementalSettings_Shared::ApplySettings()
//{
//	ApplySubtitleOptions();
//	ApplyBackgroundAudioSettings();
//	ApplyCultureSettings();
//
//	if (UEnhancedInputLocalPlayerSubsystem* System = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwningPlayer))
//	{
//		if (UEnhancedInputUserSettings* InputSettings = System->GetUserSettings())
//		{
//			InputSettings->ApplySettings();
//		}
//	}
//}
//
//void UElementalSettings_Shared::SetColorBlindStrength(int32 InColorBlindStrength)
//{
//	InColorBlindStrength = FMath::Clamp(InColorBlindStrength, 0, 10);
//	if (ColorBlindStrength != InColorBlindStrength)
//	{
//		ColorBlindStrength = InColorBlindStrength;
//		FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(
//			(EColorVisionDeficiency)(int32)ColorBlindMode, (int32)ColorBlindStrength, true, false);
//	}
//}
//
//int32 UElementalSettings_Shared::GetColorBlindStrength() const
//{
//	return ColorBlindStrength;
//}
//
//void UElementalSettings_Shared::SetColorBlindMode(EColorBlindMode InMode)
//{
//	if (ColorBlindMode != InMode)
//	{
//		ColorBlindMode = InMode;
//		FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(
//			(EColorVisionDeficiency)(int32)ColorBlindMode, (int32)ColorBlindStrength, true, false);
//	}
//}
//
//EColorBlindMode UElementalSettings_Shared::GetColorBlindMode() const
//{
//	return ColorBlindMode;
//}
//
//void UElementalSettings_Shared::ApplySubtitleOptions()
//{
//	if (USubtitleDisplaySubsystem* SubtitleSystem = USubtitleDisplaySubsystem::Get(OwningPlayer))
//	{
//		FSubtitleFormat SubtitleFormat;
//		SubtitleFormat.SubtitleTextSize = SubtitleTextSize;
//		SubtitleFormat.SubtitleTextColor = SubtitleTextColor;
//		SubtitleFormat.SubtitleTextBorder = SubtitleTextBorder;
//		SubtitleFormat.SubtitleBackgroundOpacity = SubtitleBackgroundOpacity;
//
//		SubtitleSystem->SetSubtitleDisplayOptions(SubtitleFormat);
//	}
//}
//
////////////////////////////////////////////////////////////////////////
//
//void UElementalSettings_Shared::SetAllowAudioInBackgroundSetting(ELyraAllowBackgroundAudioSetting NewValue)
//{
//	if (ChangeValueAndDirty(AllowAudioInBackground, NewValue))
//	{
//		ApplyBackgroundAudioSettings();
//	}
//}
//
//void UElementalSettings_Shared::ApplyBackgroundAudioSettings()
//{
//	if (OwningPlayer && OwningPlayer->IsPrimaryPlayer())
//	{
//		FApp::SetUnfocusedVolumeMultiplier((AllowAudioInBackground != ELyraAllowBackgroundAudioSetting::Off) ? 1.0f : 0.0f);
//	}
//}
//
////////////////////////////////////////////////////////////////////////
//
//void UElementalSettings_Shared::ApplyCultureSettings()
//{
//	if (bResetToDefaultCulture)
//	{
//		const FCulturePtr SystemDefaultCulture = FInternationalization::Get().GetDefaultCulture();
//		check(SystemDefaultCulture.IsValid());
//
//		const FString CultureToApply = SystemDefaultCulture->GetName();
//		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
//		{
//			// Clear this string
//			GConfig->RemoveKey(TEXT("Internationalization"), TEXT("Culture"), GGameUserSettingsIni);
//			GConfig->Flush(false, GGameUserSettingsIni);
//		}
//		bResetToDefaultCulture = false;
//	}
//	else if (!PendingCulture.IsEmpty())
//	{
//		// SetCurrentCulture may trigger PendingCulture to be cleared (if a culture change is broadcast) so we take a copy of it to work with
//		const FString CultureToApply = PendingCulture;
//		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
//		{
//			// Note: This is intentionally saved to the users config
//			// We need to localize text before the player logs in and very early in the loading screen
//			GConfig->SetString(TEXT("Internationalization"), TEXT("Culture"), *CultureToApply, GGameUserSettingsIni);
//			GConfig->Flush(false, GGameUserSettingsIni);
//		}
//		ClearPendingCulture();
//	}
//}
//
//void UElementalSettings_Shared::ResetCultureToCurrentSettings()
//{
//	ClearPendingCulture();
//	bResetToDefaultCulture = false;
//}
//
//const FString& UElementalSettings_Shared::GetPendingCulture() const
//{
//	return PendingCulture;
//}
//
//void UElementalSettings_Shared::SetPendingCulture(const FString& NewCulture)
//{
//	PendingCulture = NewCulture;
//	bResetToDefaultCulture = false;
//	bIsDirty = true;
//}
//
//void UElementalSettings_Shared::OnCultureChanged()
//{
//	ClearPendingCulture();
//	bResetToDefaultCulture = false;
//}
//
//void UElementalSettings_Shared::ClearPendingCulture()
//{
//	PendingCulture.Reset();
//}
//
//bool UElementalSettings_Shared::IsUsingDefaultCulture() const
//{
//	FString Culture;
//	GConfig->GetString(TEXT("Internationalization"), TEXT("Culture"), Culture, GGameUserSettingsIni);
//
//	return Culture.IsEmpty();
//}
//
//void UElementalSettings_Shared::ResetToDefaultCulture()
//{
//	ClearPendingCulture();
//	bResetToDefaultCulture = true;
//	bIsDirty = true;
//}
//
////////////////////////////////////////////////////////////////////////
//
//void UElementalSettings_Shared::ApplyInputSensitivity()
//{
//
//}
//
