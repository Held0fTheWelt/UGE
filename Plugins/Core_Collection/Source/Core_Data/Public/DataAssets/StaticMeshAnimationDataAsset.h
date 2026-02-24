// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Structs/SimpleStringMappingTableItem.h"
#include "StaticMeshAnimationDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class CORE_DATA_API UStaticMeshAnimationDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	UStaticMeshAnimationDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer),
		StaticMeshAnimationMappings()
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	/** Static mesh animation mappings for different states */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Animation")
	TArray<FSimpleStringMappingTableItem> StaticMeshAnimationMappings;

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
