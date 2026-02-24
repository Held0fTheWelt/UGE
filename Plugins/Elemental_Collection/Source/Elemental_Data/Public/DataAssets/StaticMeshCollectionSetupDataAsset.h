// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/MeshSetupDataAsset.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Structs/StaticMeshCollectionItem.h"
#include "StaticMeshCollectionSetupDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_DATA_API UStaticMeshCollectionSetupDataAsset : public UMeshSetupDataAsset
{
	GENERATED_BODY()
	
public:
	UStaticMeshCollectionSetupDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
	}

public:
	/** The static mesh to use for this setup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collection)
	FStaticMeshCollectionItem Body;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collection)
	TArray<FStaticMeshCollectionItem> StaticMeshCollection;

	virtual TArray<TSoftObjectPtr<UObject>> GetAssetObjects_Implementation() const override
	{
		TArray<TSoftObjectPtr<UObject>> AssetObjects;

		if(StaticMeshCollection.Num() == 0)
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Warning,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() called but StaticMeshCollection is empty.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return AssetObjects;
		}

		AssetObjects.Reserve((StaticMeshCollection.Num() + 1) * 3);

		if(Body.ComponentMesh.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Warning,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() called but Body.ComponentMesh is invalid.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
		}
		else
		{
			AssetObjects.Add(Body.ComponentMesh);
			UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() added Body.ComponentMesh: {MeshName}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("MeshName", Body.ComponentMesh.GetAssetName())
			);
		}

		if (Body.ComponentMaterial.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() found invalid ComponentMaterial in StaticMeshCollection.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
		}
		else
		{
			AssetObjects.Add(Body.ComponentMaterial);
			UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() added ComponentMaterial from StaticMeshCollection: {MaterialName}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("MaterialName", Body.ComponentMaterial.GetAssetName())
			);
		}

		if (Body.PhysicalMaterialOverride.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() found invalid PhysicalMaterialOverride in StaticMeshCollection.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
		}
		else
		{
			AssetObjects.Add(Body.PhysicalMaterialOverride);
			UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() added ComponentMaterialOverride from StaticMeshCollection: {MaterialName}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("MaterialName", Body.PhysicalMaterialOverride.GetAssetName())
			);
		}

		for(const FStaticMeshCollectionItem& Item : StaticMeshCollection)
		{
			if(Item.ComponentMesh.IsNull())
			{
				UE_LOGFMT(Log_PreLoadingDataAsset, Warning,
					"{Time}: {Line} {Class}: GetAssetObjects_Implementation() found invalid ComponentMesh in StaticMeshCollection.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME)
				);
			}
			else
			{
				AssetObjects.Add(Item.ComponentMesh);
				UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
					"{Time}: {Line} {Class}: GetAssetObjects_Implementation() added ComponentMesh from StaticMeshCollection: {MeshName}",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("MeshName", Item.ComponentMesh.GetAssetName())
				);
			}

			if (Item.ComponentMaterial.IsNull())
			{
				UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
					"{Time}: {Line} {Class}: GetAssetObjects_Implementation() found invalid ComponentMaterial in StaticMeshCollection.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME)
				);
			}
			else
			{
				AssetObjects.Add(Item.ComponentMaterial);
				UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
					"{Time}: {Line} {Class}: GetAssetObjects_Implementation() added ComponentMaterial from StaticMeshCollection: {MaterialName}",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("MaterialName", Item.ComponentMaterial.GetAssetName())
				);
			}

			if (Item.PhysicalMaterialOverride.IsNull())
			{
				UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
					"{Time}: {Line} {Class}: GetAssetObjects_Implementation() found invalid PhysicalMaterialOverride in StaticMeshCollection.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME)
				);
			}
			else
			{
				AssetObjects.Add(Item.PhysicalMaterialOverride);
				UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
					"{Time}: {Line} {Class}: GetAssetObjects_Implementation() added ComponentMaterialOverride from StaticMeshCollection: {MaterialName}",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("MaterialName", Item.PhysicalMaterialOverride.GetAssetName())
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
};
