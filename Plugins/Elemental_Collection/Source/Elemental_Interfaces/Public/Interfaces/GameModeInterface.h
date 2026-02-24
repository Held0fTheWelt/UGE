// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameModeInterface.generated.h"

class AController;
class AGameModeBase;
class UObject;

/** Broadcast when the game mode has finished initializing a player (controller + pawn + state). */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGameModePlayerInitialized, AGameModeBase* /*GameMode*/, AController* /*NewPlayer*/);

UINTERFACE(MinimalAPI, BlueprintType)
class UGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for game modes that provide pawn data and player initialization events.
 * Implement to drive PlayerState PawnData and to allow components (e.g. TeamCreation) to react to new players.
 */
class ELEMENTAL_INTERFACES_API IGameModeInterface
{
	GENERATED_BODY()

public:
	/** Called after standard controller/pawn init; implement for custom setup. */
	virtual void GenericPlayerInitialization(AController* NewController) = 0;

	/** Returns pawn data for the given controller (e.g. for SetPawnData on PlayerState). nullptr if not available. */
	virtual const UObject* GetPawnDataForController(AController* Controller) const { return nullptr; }

	/** Optional: return delegate to subscribe to player-initialized events. nullptr if not supported. */
	virtual FOnGameModePlayerInitialized* GetOnPlayerInitializedDelegate() { return nullptr; }
};
