// Copyright Epic Games, Inc. All Rights Reserved.
// Used by Core_System replication graph without depending on Core_Classes.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ReplicationGraphCharacterInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UReplicationGraphCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/** Implemented by the project's character class for FastShared replication. Core_System uses this without depending on Core_Classes. */
class CORE_INTERFACES_API IReplicationGraphCharacterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Replication")
	virtual bool UpdateSharedReplication() { return false; }
};
