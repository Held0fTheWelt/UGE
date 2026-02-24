// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/LocalPlayer.h"

#include "CommonLocalPlayer.generated.h"

struct FSceneViewProjectionData;

UCLASS(config = Engine, transient)
class ELEMENTAL_COMMON_API UCommonLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()

public:
	UCommonLocalPlayer();

	/** Called when the local player is assigned a player controller */
	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, class UCommonLocalPlayer* LocalPlayer, class APlayerController* PlayerController);
	FPlayerControllerSetDelegate OnPlayerControllerSet;

	/** Called when the local player is assigned a player state */
	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerStateSetDelegate, class UCommonLocalPlayer* LocalPlayer, class APlayerState* PlayerState);
	FPlayerStateSetDelegate OnPlayerStateSet;

	/** Called when the local player is assigned a player pawn */
	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerPawnSetDelegate, class UCommonLocalPlayer* LocalPlayer, class APawn* Pawn);
	FPlayerPawnSetDelegate OnPlayerPawnSet;

	FDelegateHandle CallAndRegister_OnPlayerControllerSet(FPlayerControllerSetDelegate::FDelegate Delegate);
	FDelegateHandle CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate);
	FDelegateHandle CallAndRegister_OnPlayerPawnSet(FPlayerPawnSetDelegate::FDelegate Delegate);

public:
	virtual bool GetProjectionData(FViewport* Viewport, FSceneViewProjectionData& ProjectionData, int32 StereoViewIndex) const override;

	bool IsPlayerViewEnabled() const { return bIsPlayerViewEnabled; }
	void SetIsPlayerViewEnabled(bool bInIsPlayerViewEnabled) { bIsPlayerViewEnabled = bInIsPlayerViewEnabled; }

	class UPrimaryGameLayout* GetRootUILayout() const;

private:
	bool bIsPlayerViewEnabled = true;
};