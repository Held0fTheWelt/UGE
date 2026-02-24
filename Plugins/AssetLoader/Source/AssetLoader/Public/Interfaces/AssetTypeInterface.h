// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AssetTypeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAssetTypeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASSETLOADER_API IAssetTypeInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = AssetLoader)
	const FPrimaryAssetType GetAssetType() const;
	
};
