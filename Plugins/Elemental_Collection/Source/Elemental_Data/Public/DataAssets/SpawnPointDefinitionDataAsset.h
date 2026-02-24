/*
 * Copyright (c) 2024 Yves Tanas
 * All Rights Reserved.
 *
 * This file is part of the Collections project.
 *
 * Unauthorized copying of this file, via any medium, is strictly prohibited.
 * Proprietary and confidential.
 *
 * This software may be used only as expressly authorized by the copyright holder.
 * Unless required by applicable law or agreed to in writing, software distributed
 * under this license is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 *
 * For licensing inquiries, please contact: yves.tanas@example.com
 *
 * Contributors:
 *   Yves Tanas <yves.tanas@example.com>
 *
 * -------------------------------------------------------------------------------
 * File:        SpawnPointDefinitionDataAsset.h
 * Created:     2025-06-12
 * Description: Data asset for grouping spawn points by type (camera/player/object/etc).
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Structs/SpawnPointCollection.h"
#include "SpawnPointDefinitionDataAsset.generated.h"


/**
 * USpawnPointDefinitionDataAsset
 *
 * Data asset that holds collections of spawn points, grouped by type.
 * Useful for map start locations, respawn logic, or designer-driven placement.
 */
UCLASS(BlueprintType)
class ELEMENTAL_DATA_API USpawnPointDefinitionDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	/** Standard constructor – initializes asset type name */
	USpawnPointDefinitionDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	/** All grouped spawn points for this map or context */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points")
	TArray<FSpawnPointsCollection> SpawnPoints;

	/** Returns the static asset type for this data asset class */
	virtual const FPrimaryAssetType GetAssetType_Implementation() const override
	{
		return AssetType;
	}

	static FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}

private:
	/** Static type handle for Primary Asset system (define value in .cpp!) */
	static const FPrimaryAssetType AssetType;
};
