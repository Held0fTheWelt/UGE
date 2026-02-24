// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/MeshSetupDataAsset.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "SkeletalMeshSetupDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_DATA_API USkeletalMeshSetupDataAsset : public UMeshSetupDataAsset
{
	GENERATED_BODY()
	
public:
	USkeletalMeshSetupDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		Mesh = nullptr;
		AnimInstance = nullptr;
	}

public:
	/** The skeletal mesh to use for this setup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TSoftObjectPtr<class USkeletalMesh> Mesh;
	/** The animation instance to use for this setup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TSoftClassPtr<class UAnimInstance> AnimInstance;

	virtual TArray<TSoftClassPtr<UObject>> GetAssetClasses_Implementation() const override
	{
		TArray<TSoftClassPtr<UObject>> AssetClasses;

		if(AnimInstance.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Warning,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() called with AnimInstance being null.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return AssetClasses;
		}

		AssetClasses.Add(AnimInstance);
		UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
			"{Time}: {Line} {Class}: GetAssetObjects_Implementation() called with AnimInstance: {AnimInstance}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("AnimInstance", *AnimInstance.ToString())
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

	virtual TArray<TSoftObjectPtr<UObject>> GetAssetObjects_Implementation() const override
	{
		TArray<TSoftObjectPtr<UObject>> AssetObjects;

		if(Mesh.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Warning,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() called with Mesh being null.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return AssetObjects;
		}

		AssetObjects.Add(Mesh);
		UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
			"{Time}: {Line} {Class}: GetAssetObjects_Implementation() called with Mesh: {Mesh}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Mesh", *Mesh.ToString())
		);

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
