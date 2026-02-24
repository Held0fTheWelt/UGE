// InformativeAssetClassRef.h
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
 * File:        [InformativeAssetClassRef.h]
 * Created:     [2025-06-16]
 * Description: Base struct for referencing informative DataAssets by class
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Primary/InformativePrimaryDataAsset.h"
#include "Engine/DataTable.h"
#include "InformativeAssetClassRef.generated.h"

USTRUCT(BlueprintType)
struct FInformativeAssetClassRef : public FTableRowBase
{
    GENERATED_BODY()

public:
    /** Soft reference to the informative DataAsset class */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Registry")
    TSoftClassPtr<UInformativePrimaryDataAsset> AssetClass;

    /** Returns the primary asset type as an FName, or "Invalid" if not loaded */
    FName GetDisplayName() const
    {
        if (AssetClass.IsValid())
        {
            const auto* Default = AssetClass->GetDefaultObject<UInformativePrimaryDataAsset>();
            return Default->GetPrimaryAssetType();
        }
        return FName("Invalid");
    }

    /** Returns a tooltip text with the asset type name, or an error message if invalid */
    FText GetTooltipText() const
    {
        if (AssetClass.IsValid())
        {
            const auto* Default = AssetClass->GetDefaultObject<UInformativePrimaryDataAsset>();
            return FText::Format(
                NSLOCTEXT("Assets", "AssetTypeTooltip", "Asset Type: {0}"),
                FText::FromName(Default->AssetTypeName)
            );
        }
        return NSLOCTEXT("Assets", "InvalidAsset", "Invalid or Unloaded Asset Class");
    }
};
