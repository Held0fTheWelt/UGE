// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Structs/Faction.h"
#include "FactionInformationDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CORE_DATA_API UFactionInformationDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	UFactionInformationDataAsset()
	{
		AssetTypeName = GetAssetType_Implementation();
		Factions = TArray<FFaction>();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Factions)
	TArray<FFaction> Factions;

	virtual const FPrimaryAssetType GetAssetType_Implementation() const override
	{
		return AssetType;
	}

	static FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}

private:
	static const FPrimaryAssetType AssetType;
};
