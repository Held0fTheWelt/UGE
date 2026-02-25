// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UObject/SoftObjectPath.h"
#include "AssetRegistry/AssetData.h"
#include "PawnDataProviderSubsystem.generated.h"

class AController;

/**
 * World subsystem that provides PawnData for controllers by delegating to a provider registered by Core_System.
 * Core_System sets the provider at startup so Core_Classes can get PawnData without depending on Core_System.
 */
UCLASS()
class CORE_INTERFACES_API UPawnDataProviderSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Register the provider for the given world (called by Core_System). */
	static void SetProviderForWorld(UWorld* World, UObject* Provider);

	/** Returns the provider for the given world, or nullptr. */
	static UObject* GetProviderForWorld(UWorld* World);

	/** Returns the pawn data for the given controller, or nullptr if none. */
	UFUNCTION(BlueprintCallable, Category = "PawnData")
	virtual UObject* GetPawnDataForController(const AController* Controller) const;

	/** Returns the default pawn data when no controller-specific data is available. */
	UFUNCTION(BlueprintCallable, Category = "PawnData")
	virtual UObject* GetDefaultPawnData() const;

	/** Loads and returns the pawn data at the given path, or nullptr. */
	UFUNCTION(BlueprintCallable, Category = "PawnData")
	virtual UObject* GetPawnDataFromPath(const FSoftObjectPath& Path) const;

	/** Returns true if the primary asset exists and OutAssetData is filled. */
	UFUNCTION(BlueprintCallable, Category = "PawnData")
	virtual bool GetPrimaryAssetData(FPrimaryAssetId AssetId, FAssetData& OutAssetData) const;
};
