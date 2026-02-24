// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "WidgetComponentSetupDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_DATA_API UWidgetComponentSetupDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	UWidgetComponentSetupDataAsset(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
		Scale = FVector();
		HealthbarHeight = 0.f;
	}


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Setup")
	FVector Scale; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Setup")
	float HealthbarHeight;

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
