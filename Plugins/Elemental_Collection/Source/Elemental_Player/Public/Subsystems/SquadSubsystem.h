// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SquadSubsystem.generated.h"

class APlayerState_Elemental;

/**
 * World subsystem for squad membership. Notified when a player's squad ID changes (e.g. OnRep).
 */
UCLASS()
class ELEMENTAL_PLAYER_API USquadSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Called when a player's replicated squad ID has changed. Override or bind for squad UI/logic. */
	UFUNCTION(BlueprintNativeEvent, Category = "Squad")
	void OnPlayerSquadIdChanged(APlayerState_Elemental* PlayerState);
	virtual void OnPlayerSquadIdChanged_Implementation(APlayerState_Elemental* PlayerState);
};
