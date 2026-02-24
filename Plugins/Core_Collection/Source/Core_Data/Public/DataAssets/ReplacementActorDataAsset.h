// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "ReplacementActorDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class CORE_DATA_API UReplacementActorDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	UReplacementActorDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
		// Default constructor implementation
		ReplacementActor = nullptr;
	}

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Replacement)
    TSoftObjectPtr<class AActor> ReplacementActor;

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
