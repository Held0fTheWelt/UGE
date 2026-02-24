// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Structs/CollisionSetupData.h"
#include "Engine/EngineTypes.h"
#include "CollisionControlDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CORE_DATA_API UCollisionControlDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	UCollisionControlDataAsset()
	{
		AssetTypeName = GetAssetType_Implementation();
		CollisionEnabled = ECollisionEnabled::QueryAndPhysics;
		ObjectType = ECC_WorldDynamic;
		DefaultCollisionResponse = ECR_Overlap;
		CollisionChannelSetup = TArray<FCollisionSetupData>();
		bGenerateOverlapEvents = false;
		bSetDeferredCollisionUpdatesEnabled = false;
		bSetDeferredCollisionImmediateUpdates = false;
		bUseCCD = false;
		bUseMACD = false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled = ECollisionEnabled::QueryAndPhysics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TEnumAsByte<ECollisionChannel> ObjectType = ECC_WorldDynamic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TEnumAsByte<ECollisionResponse> DefaultCollisionResponse = ECR_Ignore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TArray<FCollisionSetupData> CollisionChannelSetup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bSetDeferredCollisionUpdatesEnabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bSetDeferredCollisionImmediateUpdates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bGenerateOverlapEvents; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bUseCCD; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bUseMACD;

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
