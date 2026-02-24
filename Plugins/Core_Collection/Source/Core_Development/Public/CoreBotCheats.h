// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "GameFramework/CheatManager.h"

#include "CoreBotCheats.generated.h"

class IBotSpawnProviderInterface;
class UObject;
struct FFrame;

/** Cheats related to bots. Uses IBotSpawnProviderInterface (e.g. from GameFeature_BotSpawn). */
UCLASS(NotBlueprintable)
class CORE_DEVELOPMENT_API UCoreBotCheats final : public UCheatManagerExtension
{
	GENERATED_BODY()

public:
	UCoreBotCheats();

	// Adds a bot player
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	void AddPlayerBot();

	// Removes a random bot player
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	void RemovePlayerBot();

private:
	IBotSpawnProviderInterface* GetBotSpawnProvider() const;
};
