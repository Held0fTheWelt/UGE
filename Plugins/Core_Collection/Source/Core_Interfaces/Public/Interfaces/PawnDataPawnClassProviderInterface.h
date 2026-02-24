// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnDataPawnClassProviderInterface.generated.h"

/**
 * Interface for PawnData-like objects so callers can get the pawn class without depending on a concrete type (e.g. UPawnData_Core).
 * Implemented by UPawnData_Core (Core_System) and used by GameMode_Core (Core_Classes) via the PawnData provider.
 */
UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UPawnDataPawnClassProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class CORE_INTERFACES_API IPawnDataPawnClassProviderInterface
{
	GENERATED_BODY()

public:
	/** Returns the pawn class to spawn for this pawn data. */
	UFUNCTION(BlueprintCallable, Category = "PawnData")
	virtual UClass* GetPawnClass() const { return nullptr; }
};
