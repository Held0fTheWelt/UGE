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
 * File:        InformativePrimaryDataAsset.h
 * Created:     2025-06-12
 * Description: Abstract base class for Primary Data Assets with customizable type name.
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InformativePrimaryDataAsset.generated.h"

 /**
  * UInformativePrimaryDataAsset
  *
  * Abstract base class for all custom primary data assets that need flexible type categorization.
  * Provides support for runtime asset type assignment and easy integration with Unreal's AssetManager.
  *
  * - Inherit from this for all gameplay-specific or system-wide data asset types.
  * - Set AssetTypeName (in-editor or in constructor) for category grouping, runtime queries, and AssetManager logic.
  * - Example asset naming: DA_[AssetTypeName]_[CustomIdentifier]
  */
UCLASS(Abstract, BlueprintType)
class ASSETLOADER_API UInformativePrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/**
	 * Returns the unique primary asset ID for this asset instance.
	 * Asset type is determined by AssetTypeName, name by this object's own FName.
	 */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(GetPrimaryAssetType(), GetFName());
	}

	/**
	 * The logical type name for this data asset (used by the AssetManager for grouping/filtering).
	 * Set in the editor or by your asset class constructor.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asset Manager")
	FName AssetTypeName = "InformativeDummy";

	/**
	 * Returns the current asset type (as FPrimaryAssetType) for AssetManager queries.
	 */
	FPrimaryAssetType GetPrimaryAssetType() const
	{
		return AssetTypeName;
	}
};
