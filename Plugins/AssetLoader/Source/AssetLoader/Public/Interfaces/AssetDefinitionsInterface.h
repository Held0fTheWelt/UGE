// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AssetDefinitionsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAssetDefinitionsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASSETLOADER_API IAssetDefinitionsInterface
{
	GENERATED_BODY()

public:
	/**
	 * Returns the asset type name for this asset definition.
	 * @return The asset type name.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = AssetLoader)
	TArray<FName> GetAssetNames() const;
};
