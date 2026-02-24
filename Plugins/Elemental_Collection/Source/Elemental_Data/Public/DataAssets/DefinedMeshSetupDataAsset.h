// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "DefinedMeshSetupDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_DATA_API UDefinedMeshSetupDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	UDefinedMeshSetupDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
	}
};
