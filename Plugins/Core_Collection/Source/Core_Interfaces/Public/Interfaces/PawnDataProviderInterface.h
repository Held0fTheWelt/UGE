// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnDataProviderInterface.generated.h"

class AController;
struct FAssetData;
struct FPrimaryAssetId;
struct FSoftObjectPath;

/**
 * Interface for providing PawnData for a controller (Plan: IPawnDataProviderForController).
 * Implemented by Core_System's UCorePawnDataProviderSubsystem; Core_Classes uses only this interface
 * and UPawnDataProviderSubsystem, so it does not depend on Core_System.
 */
UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UPawnDataProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class CORE_INTERFACES_API IPawnDataProviderInterface
{
	GENERATED_BODY()

public:
	virtual UObject* GetPawnDataForController(const AController* Controller) const { return nullptr; }
	virtual UObject* GetDefaultPawnData() const { return nullptr; }
	virtual UObject* GetPawnDataFromPath(const FSoftObjectPath& Path) const { return nullptr; }
	virtual bool GetPrimaryAssetData(FPrimaryAssetId AssetId, FAssetData& OutAssetData) const { return false; }
};
