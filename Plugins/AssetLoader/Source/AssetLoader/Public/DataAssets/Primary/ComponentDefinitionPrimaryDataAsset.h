// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Primary/InformativePrimaryDataAsset.h"
#include "Structs/ComponentDefinitionTable.h"
#include "ComponentDefinitionPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ASSETLOADER_API UComponentDefinitionPrimaryDataAsset : public UInformativePrimaryDataAsset
{
	GENERATED_BODY()

public:
	UComponentDefinitionPrimaryDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType();
	}

	/**
	 * The table object for this primary data asset.
	 * This is used to identify the type of assets in the asset registry.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Definition Table")
	TSoftObjectPtr<class UDataTable> ComponentDefinitionTable;

#if WITH_EDITORONLY_DATA
	/**
	 * Extra table rows indices for the component definition table.
	 * This is used to specify additional rows that should be included in the asset and not be removed by algorithm.
	 * It is only relevant in the editor and does not affect runtime behavior.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extra Table Rows Indices")
	TArray<int32> ExtraTableRowsIndices;
#endif

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
