// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Player/CoreLocalPlayer.h"

#include "AudioMixerBlueprintLibrary.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Settings/CoreSettingsLocal.h"
#include "Settings/CoreSettingsShared.h"
#include "CommonUserSubsystem.h"
#include "Interfaces/TeamAgentInterface.h"
#include "Interfaces/LocalPlayerSharedSettingsProviderInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreLocalPlayer)

class UObject;

UCoreLocalPlayer::UCoreLocalPlayer()
{
}

void UCoreLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();

	if (UCoreSettingsLocal* LocalSettings = Cast<UCoreSettingsLocal>(GetLocalSettings()))
	{
		LocalSettings->OnAudioOutputDeviceChanged.AddUObject(this, &UCoreLocalPlayer::OnAudioOutputDeviceChanged);
	}
}

void UCoreLocalPlayer::SwitchController(class APlayerController* PC)
{
	Super::SwitchController(PC);

	OnPlayerControllerChanged(PlayerController);
}

bool UCoreLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);

	OnPlayerControllerChanged(PlayerController);

	return bResult;
}

void UCoreLocalPlayer::InitOnlineSession()
{
	OnPlayerControllerChanged(PlayerController);

	Super::InitOnlineSession();
}

void UCoreLocalPlayer::OnPlayerControllerChanged(APlayerController* NewController)
{
	// Stop listening for changes from the old controller
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (ITeamAgentInterface* ControllerAsTeamProvider = Cast<ITeamAgentInterface>(LastBoundPC.Get()))
	{
		OldTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetOnTeamIndexChangedDelegate()->RemoveAll(this);
	}

	// Grab the current team ID and listen for future changes
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (ITeamAgentInterface* ControllerAsTeamProvider = Cast<ITeamAgentInterface>(NewController))
	{
		NewTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetOnTeamIndexChangedDelegate()->AddDynamic(this, &ThisClass::OnControllerChangedTeam);
		LastBoundPC = NewController;
	}

	ITeamAgentInterface::ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
}

void UCoreLocalPlayer::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	// Do nothing, we merely observe the team of our associated player controller
}

FGenericTeamId UCoreLocalPlayer::GetGenericTeamId() const
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

FOnTeamIndexChangedDelegate* UCoreLocalPlayer::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

UObject* UCoreLocalPlayer::GetLocalSettings() const
{
	return UCoreSettingsLocal::Get();
}

UObject* UCoreLocalPlayer::GetSharedSettings() const
{
	return GetSharedSettingsObject();
}

UObject* UCoreLocalPlayer::GetSharedSettingsObject() const
{
	if (!SharedSettings)
	{
		bool bCanLoadBeforeLogin = PLATFORM_DESKTOP;
		
		if (bCanLoadBeforeLogin)
		{
			SharedSettings = UCoreSettingsShared::LoadOrCreateSettings(this);
		}
		else
		{
			SharedSettings = UCoreSettingsShared::CreateTemporarySettings(this);
		}
	}

	return SharedSettings;
}

void UCoreLocalPlayer::LoadSharedSettingsFromDisk(bool bForceLoad)
{
	FUniqueNetIdRepl CurrentNetId = GetCachedUniqueNetId();
	if (!bForceLoad && SharedSettings && CurrentNetId == NetIdForSharedSettings)
	{
		// Already loaded once, don't reload
		return;
	}

	ensure(UCoreSettingsShared::AsyncLoadOrCreateSettings(this, UCoreSettingsShared::FOnSettingsLoadedEvent::CreateLambda([this](UCoreSettingsShared* Loaded)
		{
			OnSharedSettingsLoaded(Loaded);
		})));
}

void UCoreLocalPlayer::OnSharedSettingsLoaded(UObject* LoadedOrCreatedSettings)
{
	if (ensure(LoadedOrCreatedSettings))
	{
		SharedSettings = LoadedOrCreatedSettings;
		NetIdForSharedSettings = GetCachedUniqueNetId();
	}
}

void UCoreLocalPlayer::OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId)
{
	FOnCompletedDeviceSwap DevicesSwappedCallback;
	DevicesSwappedCallback.BindUFunction(this, FName("OnCompletedAudioDeviceSwap"));
	UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(GetWorld(), InAudioOutputDeviceId, DevicesSwappedCallback);
}

void UCoreLocalPlayer::OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult)
{
	if (SwapResult.Result == ESwapAudioOutputDeviceResultState::Failure)
	{
		// TODO: Handle audio device swap failure
	}
}

void UCoreLocalPlayer::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	FGenericTeamId OldTeamID = (OldTeam == INDEX_NONE) ? FGenericTeamId::NoTeam : FGenericTeamId((uint8)OldTeam);
	FGenericTeamId NewTeamID = (NewTeam == INDEX_NONE) ? FGenericTeamId::NoTeam : FGenericTeamId((uint8)NewTeam);
	ITeamAgentInterface::ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
}
