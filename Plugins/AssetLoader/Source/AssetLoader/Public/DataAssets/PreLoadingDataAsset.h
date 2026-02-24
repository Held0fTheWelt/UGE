// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Interfaces/PreLoadingAssetInterface.h"
#include "Interfaces/AssetTypeInterface.h"
#include "PreLoadingDataAsset.generated.h"

ASSETLOADER_API DECLARE_LOG_CATEGORY_EXTERN(Log_PreLoadingDataAsset, Log, All);

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class ASSETLOADER_API UPreLoadingDataAsset : public UDataAsset, public IPreLoadingAssetInterface, public IAssetTypeInterface
{
	GENERATED_BODY()
//	
//#if WITH_EDITOR
//public:
//	virtual void SetDetailsViewData(class UDetailsView* InDetailsView) {
//		InDetailsView->PropertiesToShow.Add(AssetTypeName);
//	};
//#endif // WITH_EDITOR

protected:
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
