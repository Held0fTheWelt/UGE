// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "AnimatedTrackDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_DATA_API UAnimatedTrackDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	UAnimatedTrackDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	/** Names/identifiers of all supported vehicle physics setups for this asset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracks)
	FName ScalarParameterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tracks)
	TArray<int32> MaterialIndices;

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
