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
 * File:        [CollisionBoxConfigDataAsset.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "CollisionBoxConfigDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ELEMENTAL_DATA_API UCollisionBoxConfigDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	UCollisionBoxConfigDataAsset()
	{
		AssetTypeName = GetAssetType_Implementation();
		bSimpleAndCentered = true; // Default to true, meaning the collision box will be simple and centered
		bTurned90Degrees = false; // Default to false, meaning the collision box will not be turned 90 degrees
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collison)
	bool bSimpleAndCentered;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collison)
	bool bTurned90Degrees;

	virtual const FPrimaryAssetType GetAssetType_Implementation() const override
	{
		return AssetType;
	}

	static FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}

private:
	static const FPrimaryAssetType AssetType;

};
