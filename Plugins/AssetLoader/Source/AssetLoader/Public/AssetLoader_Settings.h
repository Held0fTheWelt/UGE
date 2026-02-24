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
 * File:        [Elemental_AssetLoader_Settings.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "AssetLoader_Settings.generated.h"
/**
 *
 */
UCLASS(Config = Game, defaultconfig)
class ASSETLOADER_API UAssetLoader_Settings : public UObject
{
	GENERATED_BODY()

public:
	UAssetLoader_Settings(const FObjectInitializer& obj) : Super(obj)
	{
		bUsePrimaryActorDefiningDataAsset = false;
		PrimaryDataAssetIdentifier = FName(TEXT("GlobalDefinitions"));
	}

	UPROPERTY(EditAnywhere, Config, Category = AssetDefinition)
	bool bUsePrimaryActorDefiningDataAsset;
	UPROPERTY(EditAnywhere, Config, Category = AssetDefinition)
	FName PrimaryDataAssetIdentifier;
	UPROPERTY(EditAnywhere, Config, Category = AssetDefinition)
	TSoftObjectPtr<class UActorDefinitionPrimaryDataAsset> PrimaryEntityDefinitionLookupTable;

};
