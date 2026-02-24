// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "Network/CommonLocalPlayer.h"
#include "Interfaces/TeamAgentInterface.h"
#include "Interfaces/CoreSettingsLocalPlayerInterface.h"
#include "Interfaces/LocalPlayerSharedSettingsProviderInterface.h"

#include "CoreLocalPlayer.generated.h"

#define UE_API CORE_CLASSES_API

struct FGenericTeamId;
struct FSwapAudioOutputResult;

class APlayerController;
class UInputMappingContext;
class UObject;
class UWorld;
struct FFrame;

/**
 * UCoreLocalPlayer
 *
 *	Local player class for Core.
 *	Adapted from ULyraLocalPlayer.
 */
UCLASS(MinimalAPI)
class UCoreLocalPlayer : public UCommonLocalPlayer, public ITeamAgentInterface, public ICoreSettingsLocalPlayerInterface, public ILocalPlayerSharedSettingsProviderInterface
{
	GENERATED_BODY()

public:

	UE_API UCoreLocalPlayer();

	//~UObject interface
	UE_API virtual void PostInitProperties() override;
	//~End of UObject interface

	//~UPlayer interface
	UE_API virtual void SwitchController(class APlayerController* PC) override;
	//~End of UPlayer interface

	//~ULocalPlayer interface
	UE_API virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
	UE_API virtual void InitOnlineSession() override;
	//~End of ULocalPlayer interface

	//~ITeamAgentInterface interface
	UE_API virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	UE_API virtual FGenericTeamId GetGenericTeamId() const override;
	UE_API virtual FOnTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ITeamAgentInterface interface

	/** Gets the local settings for this player (Core_Data). For interface use see GetSharedSettings(). */
	UFUNCTION()
	UE_API UObject* GetLocalSettings() const override;

	/** Gets the shared settings for this player (Core_Data). */
	UFUNCTION()
	UE_API UObject* GetSharedSettingsObject() const override;

	//~ILocalPlayerSharedSettingsProviderInterface
	UFUNCTION()
	UE_API virtual UObject* GetSharedSettings() const override;

	/** Starts an async request to load the shared settings, this will call OnSharedSettingsLoaded after loading or creating new ones */
	UE_API void LoadSharedSettingsFromDisk(bool bForceLoad = false);

protected:
	UE_API void OnSharedSettingsLoaded(UObject* LoadedOrCreatedSettings);

	UE_API void OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId);
	
	UFUNCTION()
	UE_API void OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult);

	UE_API void OnPlayerControllerChanged(APlayerController* NewController);

	UFUNCTION()
	UE_API void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

private:
	UPROPERTY(Transient)
	mutable TObjectPtr<UObject> SharedSettings;

	FUniqueNetIdRepl NetIdForSharedSettings;

	UPROPERTY(Transient)
	mutable TObjectPtr<const UInputMappingContext> InputMappingContext;

	UPROPERTY()
	FOnTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> LastBoundPC;
};

#undef UE_API
