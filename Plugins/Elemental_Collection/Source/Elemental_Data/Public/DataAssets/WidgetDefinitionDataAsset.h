// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "WidgetDefinitionDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_DATA_API UWidgetDefinitionDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	UWidgetDefinitionDataAsset()
	{
		AssetTypeName = GetAssetType_Implementation();
		WidgetClass = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TSoftClassPtr<class UUserWidget> WidgetClass;

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

		if(WidgetClass.IsNull())
			{
			UE_LOGFMT(Log_PreLoadingDataAsset, Warning,
				"{Time}: {Line} {Class}: GetAssetClasses_Implementation() called but WidgetClass is null.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return AssetClasses;
		}

		AssetClasses.Add(WidgetClass);

		UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
			"{Time}: {Line} {Class}: GetAssetClasses_Implementation() found WidgetClass: {WidgetClass}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("WidgetClass", WidgetClass.GetAssetName())
		);

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
	static const FPrimaryAssetType AssetType;
};
