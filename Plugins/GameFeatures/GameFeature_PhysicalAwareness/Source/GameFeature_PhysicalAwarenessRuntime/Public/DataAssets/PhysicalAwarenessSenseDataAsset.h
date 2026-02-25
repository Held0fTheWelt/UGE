// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "PhysicalAwarenessSenseDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GAMEFEATURE_PHYSICALAWARENESSRUNTIME_API UPhysicalAwarenessSenseDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	UPhysicalAwarenessSenseDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
		bDoesTrackActorsForGameManagement = false;
		bCanSenseActors = false;
		bCanTouchActors = false;
	}

	/** Default height for the UI widget (e.g. vertical offset for health bar) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Awareness")
	bool bDoesTrackActorsForGameManagement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Awareness")
	bool bCanSenseActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Awareness")
	bool bCanTouchActors;

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
