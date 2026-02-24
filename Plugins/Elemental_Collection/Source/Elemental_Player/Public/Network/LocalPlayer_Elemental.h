// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/TeamAgentInterface.h"
#include "Network/CommonLocalPlayer.h"
#include "LocalPlayer_Elemental.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_PLAYER_API ULocalPlayer_Elemental : public UCommonLocalPlayer, public ITeamAgentInterface
{
	GENERATED_BODY()

public:

	ULocalPlayer_Elemental();

	//~UObject interface
	virtual void PostInitProperties() override;
	//~End of UObject interface

	//~UPlayer interface
	virtual void SwitchController(class APlayerController* PC) override;
	//~End of UPlayer interface

	//~ULocalPlayer interface
	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, class UWorld* InWorld) override;
	virtual void InitOnlineSession() override;
	//~End of ULocalPlayer interface

	//~ILyraTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ILyraTeamAgentInterface interface

	/** Gets the local settings for this player (read from config at startup). Returns UObject* to avoid dependency on Elemental_Settings; cast to UElementalSettings_Local* in code that has access to that module. */
	UFUNCTION()
	UObject* GetLocalSettings() const;

	/** Gets the shared settings for this player (save game system). Returns UObject* to avoid dependency on Elemental_Settings; cast to UElementalSettings_Shared* in code that has access to that module. */
	UFUNCTION()
	UObject* GetSharedSettings() const;

	/** Starts an async request to load the shared settings, this will call OnSharedSettingsLoaded after loading or creating new ones */
	void LoadSharedSettingsFromDisk(bool bForceLoad = false);

protected:
	void OnSharedSettingsLoaded(UObject* LoadedOrCreatedSettings);

	void OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId);

	//UFUNCTION()
	//void OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult);

	void OnPlayerControllerChanged(APlayerController* NewController);

	UFUNCTION()
	void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

private:
	UPROPERTY(Transient)
	mutable TObjectPtr<UObject> SharedSettings;

	FUniqueNetIdRepl NetIdForSharedSettings;

	UPROPERTY(Transient)
	mutable TObjectPtr<const class UInputMappingContext> InputMappingContext;

	UPROPERTY()
	FOnTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> LastBoundPC;
};
