// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core - Settings in Core_Data

#include "Settings/CoreSettingsLocal.h"

#include "AudioModulationStatics.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/GameUserSettings.h"
#include "PerformanceStats/PerformanceStatTypes.h"
#include "HAL/PlatformMisc.h"
#include "Rendering/SlateRenderer.h"
#include "SoundControlBus.h"
#include "SoundControlBusMix.h"
#include "Subsystems/ElementalAudioProviderSubsystem.h"
#include "Interfaces/AudioSystemInterface.h"
#include "Widgets/Layout/SSafeZone.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreSettingsLocal)

namespace
{
	float CombineFrameRateLimits(float Limit1, float Limit2)
	{
		if (Limit1 <= 0.0f) { return Limit2; }
		if (Limit2 <= 0.0f) { return Limit1; }
		return FMath::Min(Limit1, Limit2);
	}
}

UCoreSettingsLocal::UCoreSettingsLocal()
{
	FrameRateLimit_OnBattery = 30.0f;
	FrameRateLimit_InMenu = 60.0f;
	FrameRateLimit_WhenBackgrounded = 30.0f;
}

UCoreSettingsLocal* UCoreSettingsLocal::Get()
{
	return GEngine ? Cast<UCoreSettingsLocal>(GEngine->GetGameUserSettings()) : nullptr;
}

void UCoreSettingsLocal::BeginDestroy()
{
	Super::BeginDestroy();
}

void UCoreSettingsLocal::SetToDefaults()
{
	Super::SetToDefaults();
}

void UCoreSettingsLocal::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);
}

void UCoreSettingsLocal::ConfirmVideoMode()
{
	Super::ConfirmVideoMode();
}

float UCoreSettingsLocal::GetEffectiveFrameRateLimit()
{
	float EffectiveFrameRateLimit = Super::GetEffectiveFrameRateLimit();

#if WITH_EDITOR
	if (GIsEditor)
	{
		return EffectiveFrameRateLimit;
	}
#endif

	if (FPlatformMisc::IsRunningOnBattery())
	{
		EffectiveFrameRateLimit = CombineFrameRateLimits(EffectiveFrameRateLimit, FrameRateLimit_OnBattery);
	}

	if (FSlateApplication::IsInitialized() && !FSlateApplication::Get().IsActive())
	{
		EffectiveFrameRateLimit = CombineFrameRateLimits(EffectiveFrameRateLimit, FrameRateLimit_WhenBackgrounded);
	}

	return EffectiveFrameRateLimit;
}

void UCoreSettingsLocal::ResetToCurrentSettings()
{
	Super::ResetToCurrentSettings();
}

void UCoreSettingsLocal::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();
	
	ApplyDisplayGamma();
	ApplySafeZoneScale();
	UpdateEffectiveFrameRateLimit();
}

int32 UCoreSettingsLocal::GetOverallScalabilityLevel() const
{
	return Super::GetOverallScalabilityLevel();
}

void UCoreSettingsLocal::SetOverallScalabilityLevel(int32 Value)
{
	Super::SetOverallScalabilityLevel(Value);
}

float UCoreSettingsLocal::GetDisplayGamma() const
{
	return DisplayGamma;
}

void UCoreSettingsLocal::SetDisplayGamma(float InGamma)
{
	DisplayGamma = InGamma;
	ApplyDisplayGamma();
}

void UCoreSettingsLocal::ApplyDisplayGamma()
{
	(void)DisplayGamma;
}

float UCoreSettingsLocal::GetFrameRateLimit_OnBattery() const
{
	return FrameRateLimit_OnBattery;
}

void UCoreSettingsLocal::SetFrameRateLimit_OnBattery(float NewLimitFPS)
{
	FrameRateLimit_OnBattery = NewLimitFPS;
	UpdateEffectiveFrameRateLimit();
}

float UCoreSettingsLocal::GetFrameRateLimit_InMenu() const
{
	return FrameRateLimit_InMenu;
}

void UCoreSettingsLocal::SetFrameRateLimit_InMenu(float NewLimitFPS)
{
	FrameRateLimit_InMenu = NewLimitFPS;
	UpdateEffectiveFrameRateLimit();
}

float UCoreSettingsLocal::GetFrameRateLimit_WhenBackgrounded() const
{
	return FrameRateLimit_WhenBackgrounded;
}

void UCoreSettingsLocal::SetFrameRateLimit_WhenBackgrounded(float NewLimitFPS)
{
	FrameRateLimit_WhenBackgrounded = NewLimitFPS;
	UpdateEffectiveFrameRateLimit();
}

float UCoreSettingsLocal::GetFrameRateLimit_Always() const
{
	return const_cast<UCoreSettingsLocal*>(this)->GetEffectiveFrameRateLimit();
}

void UCoreSettingsLocal::SetFrameRateLimit_Always(float NewLimitFPS)
{
	SetFrameRateLimit(NewLimitFPS);
	UpdateEffectiveFrameRateLimit();
}

void UCoreSettingsLocal::UpdateEffectiveFrameRateLimit()
{
	if (!IsRunningDedicatedServer())
	{
		SetFrameRateLimit(GetEffectiveFrameRateLimit());
	}
}

float UCoreSettingsLocal::GetOverallVolume() const
{
	return OverallVolume;
}

void UCoreSettingsLocal::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;
	SetVolumeForSoundClass(TEXT("Master"), InVolume);
}

float UCoreSettingsLocal::GetMusicVolume() const
{
	return MusicVolume;
}

void UCoreSettingsLocal::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;
	SetVolumeForSoundClass(TEXT("Music"), InVolume);
}

float UCoreSettingsLocal::GetSoundFXVolume() const
{
	return SoundFXVolume;
}

void UCoreSettingsLocal::SetSoundFXVolume(float InVolume)
{
	SoundFXVolume = InVolume;
	SetVolumeForSoundClass(TEXT("SFX"), InVolume);
}

float UCoreSettingsLocal::GetDialogueVolume() const
{
	return DialogueVolume;
}

void UCoreSettingsLocal::SetDialogueVolume(float InVolume)
{
	DialogueVolume = InVolume;
	SetVolumeForSoundClass(TEXT("Dialogue"), InVolume);
}

float UCoreSettingsLocal::GetVoiceChatVolume() const
{
	return VoiceChatVolume;
}

void UCoreSettingsLocal::SetVoiceChatVolume(float InVolume)
{
	VoiceChatVolume = InVolume;
	SetVolumeForSoundClass(TEXT("Voice"), InVolume);
}

void UCoreSettingsLocal::SetVolumeForSoundClass(FName ChannelName, float InVolume)
{
	if (UWorld* World = GEngine ? GEngine->GetCurrentPlayWorld() : nullptr)
	{
		ApplyVolumeToAudioBuses(World);
	}
	(void)ChannelName;
	(void)InVolume;
}

void UCoreSettingsLocal::SetVolumeForControlBus(USoundControlBus* InSoundControlBus, float InVolume)
{
	if (InSoundControlBus)
	{
		(void)InVolume;
	}
}

void UCoreSettingsLocal::LoadUserControlBusMix()
{
	bSoundControlBusMixLoaded = true;
}

void UCoreSettingsLocal::ApplyVolumeToAudioBuses(UWorld* World)
{
	if (!World) { return; }

	UElementalAudioProviderSubsystem* AudioProvider = World->GetSubsystem<UElementalAudioProviderSubsystem>();
	if (!AudioProvider) { return; }

	TScriptInterface<IAudioSystemInterface> AudioSystem = AudioProvider->GetAudioSystem();
	UObject* Provider = AudioSystem.GetObject();
	if (!Provider) { return; }

	if (UObject* Bus = IAudioSystemInterface::Execute_GetOverallVolumeControlBus(Provider))
	{
		SetVolumeForControlBus(Cast<USoundControlBus>(Bus), OverallVolume);
	}
	if (UObject* Bus = IAudioSystemInterface::Execute_GetMusicVolumeControlBus(Provider))
	{
		SetVolumeForControlBus(Cast<USoundControlBus>(Bus), MusicVolume);
	}
	if (UObject* Bus = IAudioSystemInterface::Execute_GetSoundFXVolumeControlBus(Provider))
	{
		SetVolumeForControlBus(Cast<USoundControlBus>(Bus), SoundFXVolume);
	}
	if (UObject* Bus = IAudioSystemInterface::Execute_GetDialogueVolumeControlBus(Provider))
	{
		SetVolumeForControlBus(Cast<USoundControlBus>(Bus), DialogueVolume);
	}
	if (UObject* Bus = IAudioSystemInterface::Execute_GetVoiceChatVolumeControlBus(Provider))
	{
		SetVolumeForControlBus(Cast<USoundControlBus>(Bus), VoiceChatVolume);
	}
}

void UCoreSettingsLocal::ApplyDynamicRangeToAudioMix(bool bHDRAudio, UWorld* World)
{
	if (!World) { return; }

	UElementalAudioProviderSubsystem* AudioProvider = World->GetSubsystem<UElementalAudioProviderSubsystem>();
	if (!AudioProvider) { return; }

	TScriptInterface<IAudioSystemInterface> AudioSystem = AudioProvider->GetAudioSystem();
	if (AudioSystem.GetObject())
	{
		IAudioSystemInterface::Execute_ApplyDynamicRangeEffectsChains(AudioSystem.GetObject(), bHDRAudio);
	}
}

void UCoreSettingsLocal::ApplySafeZoneScale()
{
	const float Scale = (SafeZoneScale >= 0.0f) ? SafeZoneScale : 0.0f;
	SSafeZone::SetGlobalSafeZoneScale(TOptional<float>(Scale));
}

void UCoreSettingsLocal::SetControllerPlatform(const FName InControllerPlatform)
{
	ControllerPlatform = InControllerPlatform;
}

FName UCoreSettingsLocal::GetControllerPlatform() const
{
	return ControllerPlatform;
}

void UCoreSettingsLocal::OnExperienceLoaded()
{
	ApplyNonResolutionSettings();
}

void UCoreSettingsLocal::OnHotfixDeviceProfileApplied()
{
	ApplyNonResolutionSettings();
	SaveSettings();
}

void UCoreSettingsLocal::SetAudioOutputDeviceId(const FString& InAudioOutputDeviceId)
{
	if (AudioOutputDeviceId != InAudioOutputDeviceId)
	{
		AudioOutputDeviceId = InAudioOutputDeviceId;
		OnAudioOutputDeviceChanged.Broadcast(InAudioOutputDeviceId);
	}
}

ECoreStatDisplayMode UCoreSettingsLocal::GetPerfStatDisplayState(ECoreDisplayablePerformanceStat Stat) const
{
	if (const ECoreStatDisplayMode* FoundMode = DisplayStatList.Find(Stat))
	{
		return *FoundMode;
	}
	return ECoreStatDisplayMode::Hidden;
}

void UCoreSettingsLocal::SetPerfStatDisplayState(ECoreDisplayablePerformanceStat Stat, ECoreStatDisplayMode DisplayMode)
{
	DisplayStatList.FindOrAdd(Stat) = DisplayMode;
	PerfStatSettingsChangedEvent.Broadcast();
}

void UCoreSettingsLocal::SetDesiredMobileFrameRateLimit(int32 NewLimitFPS)
{
	DesiredMobileFrameRateLimit = NewLimitFPS;
}

bool UCoreSettingsLocal::IsSupportedMobileFramePace(int32 FrameRate)
{
	return FrameRate > 0 && FrameRate <= 120;
}

int32 UCoreSettingsLocal::GetDefaultMobileFrameRate()
{
	return 60;
}

int32 UCoreSettingsLocal::GetFirstFrameRateWithQualityLimit() const
{
	return -1;
}

int32 UCoreSettingsLocal::GetLowestQualityWithFrameRateLimit() const
{
	return -1;
}

int32 UCoreSettingsLocal::GetMaxSupportedOverallQualityLevel() const
{
	return 3;
}
