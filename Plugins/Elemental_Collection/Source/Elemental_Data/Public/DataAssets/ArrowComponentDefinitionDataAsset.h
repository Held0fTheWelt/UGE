// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Structs/ArrowComponentsDefinition.h"
#include "ArrowComponentDefinitionDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_DATA_API UArrowComponentDefinitionDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	UArrowComponentDefinitionDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	/** The array of arrow component definitions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arrow Components")
	TArray<FArrowComponentsDefinition> ArrowComponents;

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
