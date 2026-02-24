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
 * File:        VehiclePhysicsSetupDataAsset.h
 * Created:     2025-06-12
 * Description: Data asset for holding lists of vehicle physics setups.
 * -------------------------------------------------------------------------------
 */

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "VehiclePhysicsSetupDataAsset.generated.h"

 /**
  * UVehiclePhysicsSetupDataAsset
  *
  * Data asset containing a list of physics setup names for vehicles.
  * Designed for modular vehicle construction and runtime lookups.
  */
UCLASS(BlueprintType)
class ELEMENTAL_DATA_API UVehiclePhysicsSetupDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	/** Constructor – sets asset type name on creation */
	UVehiclePhysicsSetupDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	/** Names/identifiers of all supported vehicle physics setups for this asset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Physics Setup")
	TArray<FName> VehiclePhysicsSetupNames;

	/** Returns the static primary asset type for this data asset class */
	virtual const FPrimaryAssetType GetAssetType_Implementation() const override
	{
		return AssetType;
	}

	static FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}

private:
	/** The static asset type for this data asset class */
	static const FPrimaryAssetType AssetType;
};
