// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "UObject/PrimaryAssetId.h"
#include "ActorCollectionDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class CORE_DATA_API UActorCollectionDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	UActorCollectionDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer),
		DefaultActor(nullptr),
		DummyActor(nullptr),
		DestroyedActor(nullptr)
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collection)
	TSoftClassPtr<class AActor> DefaultActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collection)
	TSoftClassPtr<class AActor> DummyActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collection)
	TSoftClassPtr<class AActor> DestroyedActor;

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

		if (DefaultActor.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Error,
				"{Time}: {Line} {Class}: DefaultActor is null in GetAssetClasses_Implementation().",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
		}
		else
		{
			AssetClasses.Add(DefaultActor);
			UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
				"{Time}: {Line} {Class}: DefaultActor is set to {DefaultActor}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("DefaultActor", *DefaultActor.ToString())
			);
		}

		if (DummyActor.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Error,
				"{Time}: {Line} {Class}: DummyActor is null in GetAssetClasses_Implementation().",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
		}
		else
		{
			AssetClasses.Add(DummyActor);
			UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
				"{Time}: {Line} {Class}: DummyActor is set to {DummyActor}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("DummyActor", *DummyActor.ToString())
			);
		}

		if (DestroyedActor.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Verbose,
				"{Time}: {Line} {Class}: Destroyed Actor is null in GetAssetClasses_Implementation().",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
		}
		else
		{
			AssetClasses.Add(DestroyedActor);
			UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
				"{Time}: {Line} {Class}: DestroyedActor is set to {DestroyedActor}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("DestroyedActor", *DestroyedActor.ToString())
			);
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
	static const FPrimaryAssetType AssetType;
};
