// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Settings/CoreGameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "Settings/CoreSettingsLocal.h"
#include "Settings/CoreSettingsShared.h"
#include "Settings/CustomSettings/CoreSettingValueDiscrete_PlanesInTheSky.h"
#include "Interfaces/CoreSettingsLocalPlayerInterface.h"
#include "Engine/LocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreGameSettingRegistry)

DEFINE_LOG_CATEGORY(LogCoreGameSettingRegistry);

#define LOCTEXT_NAMESPACE "Core"

//--------------------------------------
// UCoreGameSettingRegistry
//--------------------------------------

UCoreGameSettingRegistry::UCoreGameSettingRegistry()
{
}

UCoreGameSettingRegistry* UCoreGameSettingRegistry::Get(ULocalPlayer* InLocalPlayer)
{
	UCoreGameSettingRegistry* Registry = FindObject<UCoreGameSettingRegistry>(InLocalPlayer, TEXT("CoreGameSettingRegistry"), true);
	if (Registry == nullptr)
	{
		Registry = NewObject<UCoreGameSettingRegistry>(InLocalPlayer, TEXT("CoreGameSettingRegistry"));
		Registry->Initialize(InLocalPlayer);
	}

	return Registry;
}

bool UCoreGameSettingRegistry::IsFinishedInitializing() const
{
	if (!Super::IsFinishedInitializing()) { return false; }
	// Shared settings must be available (loaded or created) for the owning local player
	if (ULocalPlayer* LP = Cast<ULocalPlayer>(GetOuter()))
	{
		if (ICoreSettingsLocalPlayerInterface* SettingsPlayer = Cast<ICoreSettingsLocalPlayerInterface>(LP))
		{
			return SettingsPlayer->GetSharedSettingsObject() != nullptr;  // UObject* from interface
		}
	}
	return true;
}

void UCoreGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	VideoSettings = InitializeVideoSettings(InLocalPlayer);
	InitializeVideoSettings_FrameRates(VideoSettings, InLocalPlayer);
	RegisterSetting(VideoSettings);

	AudioSettings = InitializeAudioSettings(InLocalPlayer);
	RegisterSetting(AudioSettings);

	GameplaySettings = InitializeGameplaySettings(InLocalPlayer);
	RegisterSetting(GameplaySettings);

	MouseAndKeyboardSettings = InitializeMouseAndKeyboardSettings(InLocalPlayer);
	RegisterSetting(MouseAndKeyboardSettings);

	GamepadSettings = InitializeGamepadSettings(InLocalPlayer);
	RegisterSetting(GamepadSettings);
}

void UCoreGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();

	// Persist local (machine) settings
	if (UCoreSettingsLocal* LocalSettings = UCoreSettingsLocal::Get())
	{
		LocalSettings->ApplyNonResolutionSettings();
		LocalSettings->SaveSettings();
	}

	// Persist shared (per-player, cloud-safe) settings
	if (ULocalPlayer* LP = Cast<ULocalPlayer>(GetOuter()))
	{
		if (ICoreSettingsLocalPlayerInterface* SettingsPlayer = Cast<ICoreSettingsLocalPlayerInterface>(LP))
		{
			if (UCoreSettingsShared* SharedSettings = Cast<UCoreSettingsShared>(SettingsPlayer->GetSharedSettingsObject()))
			{
				SharedSettings->SaveSettings();
			}
		}
	}
}

UGameSettingCollection* UCoreGameSettingRegistry::InitializeVideoSettings(ULocalPlayer* InLocalPlayer)
{
	// TODO: Implement video settings initialization
	return NewObject<UGameSettingCollection>();
}

void UCoreGameSettingRegistry::InitializeVideoSettings_FrameRates(UGameSettingCollection* Screen, ULocalPlayer* InLocalPlayer)
{
	// TODO: Implement frame rate settings
}

UGameSettingCollection* UCoreGameSettingRegistry::InitializeAudioSettings(ULocalPlayer* InLocalPlayer)
{
	// TODO: Implement audio settings initialization
	return NewObject<UGameSettingCollection>();
}

UGameSettingCollection* UCoreGameSettingRegistry::InitializeGameplaySettings(ULocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* GameplayCollection = NewObject<UGameSettingCollection>();
	GameplayCollection->SetDisplayName(LOCTEXT("GameplaySettings_Name", "Spiel"));

	UCoreSettingValueDiscrete_PlanesInTheSky* PlanesInTheSkySetting = NewObject<UCoreSettingValueDiscrete_PlanesInTheSky>(GameplayCollection);
	PlanesInTheSkySetting->SetDisplayName(LOCTEXT("PlanesInTheSky_DisplayName", "Flugzeuge am Himmel"));
	PlanesInTheSkySetting->SetDescriptionRichText(LOCTEXT("PlanesInTheSky_Description", "Aktiviert oder deaktiviert die zufälligen Flugzeuge am Himmel während des Spiels."));
	GameplayCollection->AddSetting(PlanesInTheSkySetting);

	return GameplayCollection;
}

UGameSettingCollection* UCoreGameSettingRegistry::InitializeMouseAndKeyboardSettings(ULocalPlayer* InLocalPlayer)
{
	// TODO: Implement mouse and keyboard settings initialization
	return NewObject<UGameSettingCollection>();
}

UGameSettingCollection* UCoreGameSettingRegistry::InitializeGamepadSettings(ULocalPlayer* InLocalPlayer)
{
	// TODO: Implement gamepad settings initialization
	return NewObject<UGameSettingCollection>();
}

#undef LOCTEXT_NAMESPACE
