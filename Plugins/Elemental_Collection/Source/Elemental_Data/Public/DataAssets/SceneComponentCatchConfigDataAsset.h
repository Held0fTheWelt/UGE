// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Structs/CatchableSceneComponent.h"
#include "SceneComponentCatchConfigDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_DATA_API USceneComponentCatchConfigDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	USceneComponentCatchConfigDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Catch Config")
	TArray<FCatchableSceneComponent> CatchableComponents;

	/** Returns the static primary asset type for this data asset class */
	virtual const FPrimaryAssetType GetAssetType_Implementation() const override
	{
		return AssetType;
	}

	static FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}


	virtual TArray<TSoftClassPtr<UObject>> GetAssetClasses_Implementation() const override
	{
		TArray<TSoftClassPtr<UObject>> AssetClasses;

		for (const FCatchableSceneComponent& Component : CatchableComponents)
		{
			if(Component.ComponentClass.IsNull())
			{
				UE_LOGFMT(Log_PreLoadingDataAsset, Warning,
					"{Time}: {Line} {Class}: GetAssetClasses_Implementation() found a null component class.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME)
				);
				continue;
			}
			else
			{
				AssetClasses.Add(Component.ComponentClass);
				UE_LOGFMT(Log_PreLoadingDataAsset, Verbose,
					"{Time}: {Line} {Class}: GetAssetClasses_Implementation() found component class: {ComponentClass}.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("ComponentClass", Component.ComponentClass.GetAssetName())
				);
			}
		}

		UE_LOGFMT(Log_PreLoadingDataAsset, Verbose,
			"{Time}: {Line} {Class}: GetAssetObjects_Implementation() returning {Count} asset objects.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Count", AssetClasses.Num())
		);

		return AssetClasses;
	}

private:
	/** The static asset type for this data asset class */
	static const FPrimaryAssetType AssetType;
};
