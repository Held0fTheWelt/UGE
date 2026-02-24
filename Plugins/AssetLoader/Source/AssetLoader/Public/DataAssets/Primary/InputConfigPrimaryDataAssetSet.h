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
 * File:        [InputConfigPrimaryDataAssetSet.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Primary/InformativePrimaryDataAsset.h"
#include "InputConfigPrimaryDataAssetSet.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_InputConfigDataAssetOld, Log, All);

/**
 * UInputConfigPrimaryDataAssetSet
 *
 * Abstract base data asset for defining input configuration profiles.
 * Implements the IInputAssetInterface for standardized access to input metadata.
 * Designed for integration with the Unreal Asset Manager, enabling type-based discovery, filtering,
 * and runtime loading of input configuration assets.
 */
UCLASS(Abstract)
class ASSETLOADER_API UInputConfigPrimaryDataAssetSet : public UInformativePrimaryDataAsset
{
	GENERATED_BODY()

public:
	UInputConfigPrimaryDataAssetSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType();
	}

	static const FPrimaryAssetType AssetType;

	/**
	 * Retrieves the primary asset type for this asset, as set in AssetTypeName.
	 *
	 * @return FPrimaryAssetType corresponding to AssetTypeName.
	 */
	static const FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}
};
