// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Containers/Ticker.h"
#include "Subsystems/GameUIManagerSubsystem.h"

#include "CoreUIManagerSubsystem.generated.h"

class FSubsystemCollectionBase;
class UObject;

/**
 * UCoreUIManagerSubsystem
 *
 *	UI Manager Subsystem for Core.
 *	Manages UI visibility and synchronization with HUD.
 *	Adapted from LyraUIManagerSubsystem.
 */
UCLASS()
class CORE_CLASSES_API UCoreUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()

public:

	UCoreUIManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();
	
	FTSTicker::FDelegateHandle TickHandle;
};
