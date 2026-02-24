// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/LocalPlayer_Elemental.h"


#include "AudioMixerBlueprintLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "CommonUserSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LocalPlayer_Elemental)

class UObject;

ULocalPlayer_Elemental::ULocalPlayer_Elemental()
{
}

void ULocalPlayer_Elemental::PostInitProperties()
{
	Super::PostInitProperties();

	if (UObject* LocalSettings = GetLocalSettings())
	{
		// Optional: bind to settings events in a module that knows Elemental_Settings (e.g. Core_Setup)
	}
}

void ULocalPlayer_Elemental::SwitchController(class APlayerController* PC)
{
	Super::SwitchController(PC);

	OnPlayerControllerChanged(PlayerController);
}

bool ULocalPlayer_Elemental::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);

	OnPlayerControllerChanged(PlayerController);

	return bResult;
}

void ULocalPlayer_Elemental::InitOnlineSession()
{
	OnPlayerControllerChanged(PlayerController);

	Super::InitOnlineSession();
}

void ULocalPlayer_Elemental::OnPlayerControllerChanged(APlayerController* NewController)
{
	// Stop listening for changes from the old controller
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (ITeamAgentInterface* ControllerAsTeamProvider = Cast<ITeamAgentInterface>(LastBoundPC.Get()))
	{
		OldTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().RemoveAll(this);
	}

	// Grab the current team ID and listen for future changes
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (ITeamAgentInterface* ControllerAsTeamProvider = Cast<ITeamAgentInterface>(NewController))
	{
		NewTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnControllerChangedTeam);
		LastBoundPC = NewController;
	}

	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
}

void ULocalPlayer_Elemental::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	// Do nothing, we merely observe the team of our associated player controller
}

FGenericTeamId ULocalPlayer_Elemental::GetGenericTeamId() const
{
	if (ITeamAgentInterface* ControllerAsTeamProvider = Cast<ITeamAgentInterface>(PlayerController))
	{
		return ControllerAsTeamProvider->GetGenericTeamId();
	}
	else
	{
		return FGenericTeamId::NoTeam;
	}
}

FOnTeamIndexChangedDelegate* ULocalPlayer_Elemental::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

UObject* ULocalPlayer_Elemental::GetLocalSettings() const
{
	// Implemented by a subclass or external module that has Elemental_Settings (e.g. Core_Setup); base returns nullptr.
	return nullptr;
}

UObject* ULocalPlayer_Elemental::GetSharedSettings() const
{
	// SharedSettings is set via OnSharedSettingsLoaded by a module that has Elemental_Settings
	return SharedSettings;
}

void ULocalPlayer_Elemental::LoadSharedSettingsFromDisk(bool bForceLoad)
{
	FUniqueNetIdRepl CurrentNetId = GetCachedUniqueNetId();
	if (!bForceLoad && SharedSettings && CurrentNetId == NetIdForSharedSettings)
	{
		// Already loaded once, don't reload
		return;
	}

	//ensure(UElementalSettings_Shared::AsyncLoadOrCreateSettings(this, UElementalSettings_Shared::FOnSettingsLoadedEvent::CreateUObject(this, &ULocalPlayer_Elemental::OnSharedSettingsLoaded)));
}

void ULocalPlayer_Elemental::OnSharedSettingsLoaded(UObject* LoadedOrCreatedSettings)
{
	if (ensure(LoadedOrCreatedSettings))
	{
		SharedSettings = LoadedOrCreatedSettings;
		NetIdForSharedSettings = GetCachedUniqueNetId();
	}
}
void ULocalPlayer_Elemental::OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId)
{
	FOnCompletedDeviceSwap DevicesSwappedCallback;
	DevicesSwappedCallback.BindUFunction(this, FName("OnCompletedAudioDeviceSwap"));
	//UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(GetWorld(), InAudioOutputDeviceId, DevicesSwappedCallback);
}

//void ULocalPlayer_Elemental::OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult)
//{
//	//if (SwapResult.Result == ESwapAudioOutputDeviceResultState::Failure)
//	//{
//	//}
//}

void ULocalPlayer_Elemental::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
}

