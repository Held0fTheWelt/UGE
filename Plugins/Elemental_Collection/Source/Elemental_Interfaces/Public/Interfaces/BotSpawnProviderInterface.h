// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BotSpawnProviderInterface.generated.h"

/**
 * Interface for providing bot spawn configuration and actions.
 * Implemented by GameFeature_BotSpawn (e.g. BotSpawnProviderComponent) so that
 * Elemental_Classes (BotCreationComponent) and Core_Development (Bot-Cheats) can
 * use bot spawn logic without depending on Core_Classes or concrete GameFeature modules.
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UBotSpawnProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IBotSpawnProviderInterface
{
	GENERATED_BODY()

public:
	/** Returns the desired number of bots (e.g. from OptionsString "NumBots" or config). */
	UFUNCTION(BlueprintCallable, Category = "BotSpawn")
	virtual int32 GetDesiredBotCount() const { return 0; }

	/** Returns true if bots should be spawned (e.g. for PIE/Server). */
	UFUNCTION(BlueprintCallable, Category = "BotSpawn")
	virtual bool ShouldSpawnBots() const { return true; }

	/** Spawns one bot. Used by BotCreationComponent and by cheats (AddPlayerBot). */
	UFUNCTION(BlueprintCallable, Category = "BotSpawn")
	virtual void SpawnOneBot() {}

	/** Removes one bot. Used by BotCreationComponent and by cheats (RemovePlayerBot). */
	UFUNCTION(BlueprintCallable, Category = "BotSpawn")
	virtual void RemoveOneBot() {}
};
