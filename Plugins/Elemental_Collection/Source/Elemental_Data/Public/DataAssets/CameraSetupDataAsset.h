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
 * File:        CameraSetupPrimaryDataAsset.h
 * Created:     2025-06-12
 * Description: Primary Data Asset defining available camera setups and view types.
 * -------------------------------------------------------------------------------
 */

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Camera/PlayerCameraManager.h"
#include "Enums/CameraViewType.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Structs/ViewTypeConfiguration.h"
#include "Structs/PostProcessConfig.h"
#include "CameraSetupDataAsset.generated.h"

 /**
  * UCameraSetupPrimaryDataAsset
  *
  * Data asset that defines camera view types and their configurations for an actor or system.
  * Use to drive runtime camera logic, customization, and designer-driven camera setups.
  */
UCLASS(BlueprintType)
class ELEMENTAL_DATA_API UCameraSetupDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	/** Standard constructor */
	UCameraSetupDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
		, DefaultViewType(ECameraViewType::CVT_NONE)
		, TargetBlendTime(0.5f)
		, TargetBlendFunction(EViewTargetBlendFunction::VTBlend_Cubic)
		, ViewTypeConfigurations()
		, bUseSkeletalMeshComponent(false)
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	/** The default view type to use if none is selected at runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setup")
	ECameraViewType DefaultViewType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setup")
	float TargetBlendTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setup")
	TEnumAsByte<EViewTargetBlendFunction> TargetBlendFunction = EViewTargetBlendFunction::VTBlend_Cubic;

	/** All supported camera view type configurations for this asset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Setup")
	TArray<FViewTypeConfiguration> ViewTypeConfigurations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh Component")
	bool bUseSkeletalMeshComponent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
	bool bHasPostProcessComponent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
	FVector PostProcessSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
	TArray<FPostProcessConfig> PostProcessConfigs;

	int32 GetViewTypeIndex(ECameraViewType ViewType) const
	{
		for (int32 i = 0; i < ViewTypeConfigurations.Num(); ++i)
		{
			if (ViewTypeConfigurations[i].ViewType == ViewType)
			{
				return i;
			}
		}
		return -1; // Default implementation, can be overridden
	}

	/** Returns the static asset type for this class (for Primary Asset system) */
	virtual const FPrimaryAssetType GetAssetType_Implementation() const override
	{
		return AssetType;
	}

	static FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}

	virtual TArray<TSoftObjectPtr<UObject>> GetAssetObjects_Implementation() const override
	{
		TArray<TSoftObjectPtr<UObject>> AssetObjects;

		if(PostProcessConfigs.Num() > 0)
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Verbose,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() called. PostProcessConfigs has {Count} entries.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Count", PostProcessConfigs.Num())
			);

			for (const FPostProcessConfig& Config : PostProcessConfigs)
			{
				UE_LOGFMT(Log_PreLoadingDataAsset, Verbose,
					"{Time}: {Line} {Class}: Processing PostProcessConfig with {Count} materials.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("Count", Config.PostProcessMaterials.Num())
				);

				for (const TSoftObjectPtr<UMaterialInterface>& Material : Config.PostProcessMaterials)
				{
					if (Material.IsValid())
					{
						AssetObjects.Add(Material);
					}
				}
			}
		}

		UE_LOGFMT(Log_PreLoadingDataAsset, Verbose,
			"{Time}: {Line} {Class}: GetAssetObjects_Implementation() returning {Count} asset objects.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Count", AssetObjects.Num())
		);

		return AssetObjects;
	}

private:
	/** Static type handle for the AssetManager (define value in .cpp!) */
	static const FPrimaryAssetType AssetType;
};
