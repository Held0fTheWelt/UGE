// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core_Interfaces/Public/Subsystems/PawnDataProviderSubsystem.h"
#include "CorePawnDataProviderSubsystem.generated.h"

/**
 * Core implementation of UPawnDataProviderSubsystem; uses UAssetManager_Core and UPawnData_Core.
 * Registers itself as the PawnData provider for the world so Core_Interfaces' subsystem can delegate to it.
 */
UCLASS(meta = (Interfaces = "PawnDataProviderInterface"))
class CORE_SYSTEM_API UCorePawnDataProviderSubsystem : public UPawnDataProviderSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual UObject* GetPawnDataForController(const AController* Controller) const override;
	virtual UObject* GetDefaultPawnData() const override;
	virtual UObject* GetPawnDataFromPath(const FSoftObjectPath& Path) const override;
	virtual bool GetPrimaryAssetData(FPrimaryAssetId AssetId, FAssetData& OutAssetData) const override;
};
