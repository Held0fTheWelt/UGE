// Copyright Epic Games, Inc. All Rights Reserved.
// Used by Core_System replication graph to get character class without depending on Core_Classes.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ReplicationGraphCharacterProviderInterface.generated.h"

class UClass;

UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UReplicationGraphCharacterProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/** Implemented by game state to provide the replication character class. Core_System uses this without depending on Core_Classes. */
class CORE_INTERFACES_API IReplicationGraphCharacterProviderInterface
{
	GENERATED_BODY()

public:
	/** Returns the character class used for replication graph (e.g. ACoreCharacter). If null, Core_System falls back to ACharacter. */
	UFUNCTION(BlueprintCallable, Category = "Replication")
	virtual UClass* GetReplicationGraphCharacterClass() const { return nullptr; }
};
