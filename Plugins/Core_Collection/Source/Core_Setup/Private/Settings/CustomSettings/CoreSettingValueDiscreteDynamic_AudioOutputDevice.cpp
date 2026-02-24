// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for Core

#include "Settings/CustomSettings/CoreSettingValueDiscreteDynamic_AudioOutputDevice.h"

#include "AudioDeviceNotificationSubsystem.h"
#include "Settings/CoreSettingsLocal.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreSettingValueDiscreteDynamic_AudioOutputDevice)

#define LOCTEXT_NAMESPACE "CoreSettings"

void UCoreSettingValueDiscreteDynamic_AudioOutputDevice::OnInitialized()
{
	Super::OnInitialized();

	DevicesObtainedCallback.BindUFunction(this, FName("OnAudioOutputDevicesObtained"));
	DevicesSwappedCallback.BindUFunction(this, FName("OnCompletedDeviceSwap"));

	if (UAudioDeviceNotificationSubsystem* AudioDeviceNotifSubsystem = UAudioDeviceNotificationSubsystem::Get())
	{
		AudioDeviceNotifSubsystem->DeviceAddedNative.AddUObject(this, &UCoreSettingValueDiscreteDynamic_AudioOutputDevice::DeviceAddedOrRemoved);
		AudioDeviceNotifSubsystem->DeviceRemovedNative.AddUObject(this, &UCoreSettingValueDiscreteDynamic_AudioOutputDevice::DeviceAddedOrRemoved);
		AudioDeviceNotifSubsystem->DefaultRenderDeviceChangedNative.AddUObject(this, &UCoreSettingValueDiscreteDynamic_AudioOutputDevice::DefaultDeviceChanged);
	}

	UAudioMixerBlueprintLibrary::GetAvailableAudioOutputDevices(this, DevicesObtainedCallback);
}

void UCoreSettingValueDiscreteDynamic_AudioOutputDevice::OnAudioOutputDevicesObtained(const TArray<FAudioOutputDeviceInfo>& AvailableDevices)
{
	int32 NewSize = AvailableDevices.Num();
	OutputDevices.Reset(NewSize++);
	OutputDevices.Append(AvailableDevices);

	OptionValues.Reset(NewSize);
	OptionDisplayTexts.Reset(NewSize);

	// Placeholder - needs to be first option so we can format the default device string later
	AddDynamicOption(TEXT(""), FText::GetEmpty());
	FString SystemDefaultDeviceName;

	for (const FAudioOutputDeviceInfo& DeviceInfo : OutputDevices)
	{
		if (!DeviceInfo.DeviceId.IsEmpty() && !DeviceInfo.Name.IsEmpty())
		{
			// System Default 
			if (DeviceInfo.bIsSystemDefault)
			{
				SystemDefaultDeviceId = DeviceInfo.DeviceId;
				SystemDefaultDeviceName = DeviceInfo.Name;
			}

			// Current Device
			if (DeviceInfo.bIsCurrentDevice)
			{
				CurrentDeviceId = DeviceInfo.DeviceId;
			}

			// Add the menu option
			AddDynamicOption(DeviceInfo.DeviceId, FText::FromString(DeviceInfo.Name));
		}
	}

	OptionDisplayTexts[0] = FText::Format(LOCTEXT("DefaultAudioOutputDevice", "Default Output - {0}"), FText::FromString(SystemDefaultDeviceName));
	SetDefaultValueFromString(TEXT(""));
	RefreshEditableState();
}

void UCoreSettingValueDiscreteDynamic_AudioOutputDevice::OnCompletedDeviceSwap(const FSwapAudioOutputResult& SwapResult)
{
	// TODO: Handle device swap completion if needed
}

void UCoreSettingValueDiscreteDynamic_AudioOutputDevice::DeviceAddedOrRemoved(FString DeviceId)
{
	UAudioMixerBlueprintLibrary::GetAvailableAudioOutputDevices(this, DevicesObtainedCallback);
}

void UCoreSettingValueDiscreteDynamic_AudioOutputDevice::DefaultDeviceChanged(EAudioDeviceChangedRole InRole, FString DeviceId)
{
	UAudioMixerBlueprintLibrary::GetAvailableAudioOutputDevices(this, DevicesObtainedCallback);
}

void UCoreSettingValueDiscreteDynamic_AudioOutputDevice::SetDiscreteOptionByIndex(int32 Index)
{
	Super::SetDiscreteOptionByIndex(Index);

	ULocalPlayer* OwningLocalPlayer = nullptr;
	for (UObject* O = GetOuter(); O; O = O->GetOuter())
	{
		OwningLocalPlayer = Cast<ULocalPlayer>(O);
		if (OwningLocalPlayer) { break; }
	}
	if (OwningLocalPlayer)
	{
		FString RequestedAudioDeviceId = GetValueAsString();
		UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(OwningLocalPlayer, RequestedAudioDeviceId, DevicesSwappedCallback);
	}
	// Persist to CoreSettingsLocal so it survives restart
	if (UCoreSettingsLocal* LocalSettings = UCoreSettingsLocal::Get())
	{
		LocalSettings->SetAudioOutputDeviceId(GetValueAsString());
	}
}

#undef LOCTEXT_NAMESPACE
