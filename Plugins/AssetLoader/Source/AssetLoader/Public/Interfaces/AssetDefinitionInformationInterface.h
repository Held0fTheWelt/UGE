// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AssetDefinitionInformationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAssetDefinitionInformationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASSETLOADER_API IAssetDefinitionInformationInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = AssetLoader)
	FName GetDefinitionAssetName() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = AssetLoader)
	void SetDefinitionAssetName(FName AssetName);
};
