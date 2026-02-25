// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core - Settings types in Core_Data

#pragma once

#include "GameFramework/GameUserSettings.h"
#include "InputCoreTypes.h"
#include "PerformanceStats/PerformanceStatTypes.h"

#include "CoreSettingsLocal.generated.h"

enum class ECommonInputType : uint8;

class ULocalPlayer;
class UObject;
class USoundControlBus;
class USoundControlBusMix;
struct FFrame;

/**
 * UCoreSettingsLocal
 *
 *	Local settings for Core (machine-specific).
 *	Adapted from LyraSettingsLocal.
 */
UCLASS()
class CORE_DATA_API UCoreSettingsLocal : public UGameUserSettings
{
	GENERATED_BODY()

public:

	UCoreSettingsLocal();

	static UCoreSettingsLocal* Get();

	//~UObject interface
	virtual void BeginDestroy() override;
	//~End of UObject interface

	//~UGameUserSettings interface
	virtual void SetToDefaults() override;
	virtual void LoadSettings(bool bForceReload) override;
	virtual void ConfirmVideoMode() override;
	virtual float GetEffectiveFrameRateLimit() override;
	virtual void ResetToCurrentSettings() override;
	virtual void ApplyNonResolutionSettings() override;
	virtual int32 GetOverallScalabilityLevel() const override;
	virtual void SetOverallScalabilityLevel(int32 Value) override;
	//~End of UGameUserSettings interface

	//////////////////////////////////////////////////////////////////
	// Brightness/Gamma
public:
	UFUNCTION()
	float GetDisplayGamma() const;
	UFUNCTION()
	void SetDisplayGamma(float InGamma);

private:
	void ApplyDisplayGamma();
	
	UPROPERTY(Config)
	float DisplayGamma = 2.2;

	//////////////////////////////////////////////////////////////////
	// Display
public:
	UFUNCTION()
	float GetFrameRateLimit_OnBattery() const;
	UFUNCTION()
	void SetFrameRateLimit_OnBattery(float NewLimitFPS);

	UFUNCTION()
	float GetFrameRateLimit_InMenu() const;
	UFUNCTION()
	void SetFrameRateLimit_InMenu(float NewLimitFPS);

	UFUNCTION()
	float GetFrameRateLimit_WhenBackgrounded() const;
	UFUNCTION()
	void SetFrameRateLimit_WhenBackgrounded(float NewLimitFPS);

	UFUNCTION()
	float GetFrameRateLimit_Always() const;
	UFUNCTION()
	void SetFrameRateLimit_Always(float NewLimitFPS);

protected:
	void UpdateEffectiveFrameRateLimit();

private:
	UPROPERTY(Config)
	float FrameRateLimit_OnBattery;
	UPROPERTY(Config)
	float FrameRateLimit_InMenu;
	UPROPERTY(Config)
	float FrameRateLimit_WhenBackgrounded;

	//////////////////////////////////////////////////////////////////
	// Audio - Volume
public:
	UFUNCTION()
	float GetOverallVolume() const;
	UFUNCTION()
	void SetOverallVolume(float InVolume);

	UFUNCTION()
	float GetMusicVolume() const;
	UFUNCTION()
	void SetMusicVolume(float InVolume);

	UFUNCTION()
	float GetSoundFXVolume() const;
	UFUNCTION()
	void SetSoundFXVolume(float InVolume);

	UFUNCTION()
	float GetDialogueVolume() const;
	UFUNCTION()
	void SetDialogueVolume(float InVolume);

	UFUNCTION()
	float GetVoiceChatVolume() const;
	UFUNCTION()
	void SetVoiceChatVolume(float InVolume);

	/** Applies current volume settings to the audio system via IAudioVolumeBusesProviderInterface (Elemental). Call when World is available (e.g. after game start). */
	void ApplyVolumeToAudioBuses(class UWorld* World);

	/** Applies HDR/LDR audio effect chains via IAudioMixEffectsInterface (Elemental). Call when user toggles HDR audio. */
	void ApplyDynamicRangeToAudioMix(bool bHDRAudio, class UWorld* World);

private:
	void SetVolumeForSoundClass(FName ChannelName, float InVolume);
	void SetVolumeForControlBus(USoundControlBus* InSoundControlBus, float InVolume);
	void LoadUserControlBusMix();

	UPROPERTY(Config)
	float OverallVolume = 1.0f;
	UPROPERTY(Config)
	float MusicVolume = 1.0f;
	UPROPERTY(Config)
	float SoundFXVolume = 1.0f;
	UPROPERTY(Config)
	float DialogueVolume = 1.0f;
	UPROPERTY(Config)
	float VoiceChatVolume = 1.0f;

	UPROPERTY(Transient)
	TMap<FName/*SoundClassName*/, TObjectPtr<USoundControlBus>> ControlBusMap;

	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> ControlBusMix = nullptr;

	UPROPERTY(Transient)
	bool bSoundControlBusMixLoaded;

	//////////////////////////////////////////////////////////////////
	// Safezone
public:
	UFUNCTION()
	bool IsSafeZoneSet() const { return SafeZoneScale != -1; }
	UFUNCTION()
	float GetSafeZone() const { return SafeZoneScale >= 0 ? SafeZoneScale : 0; }
	UFUNCTION()
	void SetSafeZone(float Value) { SafeZoneScale = Value; ApplySafeZoneScale(); }

	void ApplySafeZoneScale();
private:
	UPROPERTY(Config)
	float SafeZoneScale = -1;

	//////////////////////////////////////////////////////////////////
	// Keybindings
public:
	
	UFUNCTION()
	void SetControllerPlatform(const FName InControllerPlatform);
	UFUNCTION()
	FName GetControllerPlatform() const;

private:
	UPROPERTY(Config)
	FName ControllerPlatform;

	UPROPERTY(Config)
	FName ControllerPreset = TEXT("Default");

	UPROPERTY(Config)
	FName InputConfigName = TEXT("Default");

	//////////////////////////////////////////////////////////////////
	// Experience
public:
	void OnExperienceLoaded();
	void OnHotfixDeviceProfileApplied();

	//////////////////////////////////////////////////////////////////
	// Performance stats
public:
	UFUNCTION()
	ECoreStatDisplayMode GetPerfStatDisplayState(ECoreDisplayablePerformanceStat Stat) const;
	UFUNCTION()
	void SetPerfStatDisplayState(ECoreDisplayablePerformanceStat Stat, ECoreStatDisplayMode DisplayMode);

	DECLARE_EVENT(UCoreSettingsLocal, FPerfStatSettingsChanged);
	FPerfStatSettingsChanged& OnPerfStatDisplayStateChanged() { return PerfStatSettingsChangedEvent; }

private:
	UPROPERTY(Config)
	TMap<ECoreDisplayablePerformanceStat, ECoreStatDisplayMode> DisplayStatList;

	FPerfStatSettingsChanged PerfStatSettingsChangedEvent;

	//////////////////////////////////////////////////////////////////
	// Audio Output Device
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAudioOutputDeviceChanged, const FString& /*InAudioOutputDeviceId*/);
	FOnAudioOutputDeviceChanged OnAudioOutputDeviceChanged;

	UFUNCTION()
	FString GetAudioOutputDeviceId() const { return AudioOutputDeviceId; }
	UFUNCTION()
	void SetAudioOutputDeviceId(const FString& InAudioOutputDeviceId);

private:
	UPROPERTY(Config)
	FString AudioOutputDeviceId;

	//////////////////////////////////////////////////////////////////
	// Replays
public:
	UFUNCTION(BlueprintCallable, Category = "Replays")
	int32 GetNumberOfReplaysToKeep() const { return NumberOfReplaysToKeep; }
	UFUNCTION(BlueprintCallable, Category = "Replays")
	void SetNumberOfReplaysToKeep(int32 InCount) { NumberOfReplaysToKeep = FMath::Clamp(InCount, 0, 1000); }

private:
	UPROPERTY(Config, meta = (ClampMin = "0", ClampMax = "1000"))
	int32 NumberOfReplaysToKeep = 10;

	//////////////////////////////////////////////////////////////////
	// PlanesInTheSky
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool IsPlanesInTheSkyEnabled() const { return bPlanesInTheSkyEnabled; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SetPlanesInTheSkyEnabled(bool bEnabled) { bPlanesInTheSkyEnabled = bEnabled; }

private:
	UPROPERTY(Config, meta = (DisplayName = "Planes In The Sky Enabled"))
	bool bPlanesInTheSkyEnabled = true;

	//////////////////////////////////////////////////////////////////
	// Mobile FPS
public:
	UFUNCTION()
	int32 GetDesiredMobileFrameRateLimit() const { return DesiredMobileFrameRateLimit; }
	UFUNCTION()
	void SetDesiredMobileFrameRateLimit(int32 NewLimitFPS);

	static bool IsSupportedMobileFramePace(int32 FrameRate);
	static int32 GetDefaultMobileFrameRate();

	UFUNCTION()
	int32 GetFirstFrameRateWithQualityLimit() const;
	UFUNCTION()
	int32 GetLowestQualityWithFrameRateLimit() const;
	UFUNCTION()
	int32 GetMaxSupportedOverallQualityLevel() const;

private:
	UPROPERTY(Config)
	int32 DesiredMobileFrameRateLimit = 60;
};
