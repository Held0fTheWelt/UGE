// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Structs/MeshComponentDefinition.h"
#include "BarrelConfigDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_DATA_API UBarrelConfigDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	UBarrelConfigDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
		BarrelComponentDefinition = FMeshComponentDefinition();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrel Configuration")
	FMeshComponentDefinition BarrelComponentDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barrel Configuration")
	FName BarrelSocketName = TEXT("BarrelFireSocket");

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
