// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Structs/VehicleLightSetup.h"
#include "VehicleLightsSetupDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class CORE_DATA_API UVehicleLightsSetupDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	UVehicleLightsSetupDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
		// Default constructor implementation
		LightSetup = FVehicleLightSetup();
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Lights)
	FVehicleLightSetup LightSetup;

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
