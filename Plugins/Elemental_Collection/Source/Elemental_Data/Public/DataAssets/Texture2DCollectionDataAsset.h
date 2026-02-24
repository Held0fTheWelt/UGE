// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Texture2DCollectionDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ELEMENTAL_DATA_API UTexture2DCollectionDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	UTexture2DCollectionDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
		Textures.Empty();
	}

	/** Names/identifiers of all supported vehicle physics setups for this asset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Textures)
	TArray<TSoftObjectPtr<class UTexture2D>> Textures;

	/** Returns the static primary asset type for this data asset class */
	virtual const FPrimaryAssetType GetAssetType_Implementation() const override
	{
		return AssetType;
	}

	static FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}

	virtual TArray<TSoftObjectPtr<UObject>> GetAssetObjects_Implementation() const override
	{
		TArray<TSoftObjectPtr<UObject>> AssetObjects;

		if(Textures.Num() == 0)
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Warning,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() called but Textures array is empty.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return AssetObjects;
		}

		for(const TSoftObjectPtr<UTexture2D>& Texture : Textures)
		{
			if (Texture.IsNull())
			{
				UE_LOGFMT(Log_PreLoadingDataAsset, Warning,
					"{Time}: {Line} {Class}: GetAssetObjects_Implementation() found a null texture in the collection.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME)
				);
				continue;
			}
			else
			{
				AssetObjects.Add(Texture);
				UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
					"{Time}: {Line} {Class}: GetAssetObjects_Implementation() added texture {TextureName} to asset objects.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("TextureName", Texture.GetAssetName())
				);
			}
		}

		UE_LOGFMT(Log_PreLoadingDataAsset, Verbose,
			"{Time}: {Line} {Class}: GetAssetObjects_Implementation() returning {Count} asset objects.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Count", AssetObjects.Num())
		);

		return AssetObjects;
	}

private:
	/** The static asset type for this data asset class */
	static const FPrimaryAssetType AssetType;
};
