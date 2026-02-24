// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Structs/GuidManagerEntry.h"
#include "GuidDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CORE_DATA_API UGuidDataAsset : public UPreLoadingDataAsset
{
    GENERATED_BODY()

public:
    UGuidDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
    {
		AssetTypeName = GetAssetType_Implementation();
		GuidEntries.Empty(); // Initialisiere die Einträge als leer
    }

    // Asset‑Type für AssetManager
    static const FPrimaryAssetType AssetType;// = FPrimaryAssetType(TEXT("GuidRegistry"));;

    // Die Einträge für MapName → Path → GUID
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GUID Registry")
    TArray<FGuidManagerEntry> GuidEntries;

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