// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Primary/InformativePrimaryDataAsset.h"
#include "Interfaces/AssetDefinitionsInterface.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Structs/ActorDefinitionTable.h"
#include "ActorDefinitionPrimaryDataAsset.generated.h"

DEFINE_LOG_CATEGORY_STATIC(Log_ActorDefinitionDataAsset, Log, All);

/**
 * 
 */
UCLASS()
class ASSETLOADER_API UActorDefinitionPrimaryDataAsset : public UInformativePrimaryDataAsset, public IAssetDefinitionsInterface
{
	GENERATED_BODY()

public:
	UActorDefinitionPrimaryDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType();
	}

	/**
	 * The table object for this primary data asset.
	 * This is used to identify the type of assets in the asset registry.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Definition Table")
	TSoftObjectPtr<class UDataTable> ActorDefinition;

	/**
	 * Returns the asset names from the ActorDefinition table.
	 * This is used to populate the asset registry with the names of the actor definitions.
	 *
	 * @return An array of FName containing the names of the actor definitions.
	 */
	virtual TArray<FName> GetAssetNames_Implementation() const override
	{
		TArray<FName> Names;

		if(ActorDefinition.IsNull())
		{
			UE_LOGFMT(Log_ActorDefinitionDataAsset, Warning, "{Time}: {Line} {Class}: ActorDefinition is null.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return Names;
		}

		if(!ActorDefinition.IsValid())
		{
			ActorDefinition.LoadSynchronous();
		}

		static const FString ContextString(TEXT("ExtractActorDefinitionNames"));
		TArray<FActorDefinitionTable*> Rows;
		ActorDefinition->GetAllRows(ContextString, Rows);

		for (FActorDefinitionTable* Row : Rows)
		{
			if (Row)
			{
				Names.Add(Row->ActorDefinitionName);
			}
		}

		UE_LOGFMT(Log_ActorDefinitionDataAsset, Verbose, "{Time}: {Line} {Class}: Retrieved {Count} actor definition names.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Count", Names.Num()),
			("Time", GET_CURRENT_TIME));

		return Names;
	}

	static const FPrimaryAssetType AssetType;

	/**
	 * Retrieves the primary asset type for this asset, as set in AssetTypeName.
	 *
	 * @return FPrimaryAssetType corresponding to AssetTypeName.
	 */
	static const FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}
};
