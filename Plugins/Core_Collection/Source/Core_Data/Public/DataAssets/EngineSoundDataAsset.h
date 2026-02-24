// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Structs/AudioComponentSetup.h"
#include "Structs/SoundSetup.h"
#include "EngineSoundDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class CORE_DATA_API UEngineSoundDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	UEngineSoundDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	// Array of audio components to be set up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Components")
	TArray<FAudioComponentSetup> AudioComponents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Setup")
	TArray<FSoundSetup> SoundSetups;

	// --- Spool Configuration ---

	/** Throttle value at engine idle (after spool-up completes) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spool Configuration", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float IdleThrottleValue = 0.2f;

	/** Duration in seconds for engine to spool up to idle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spool Configuration", meta = (ClampMin = "0.1", ClampMax = "30.0"))
	float EngineSpoolUpDuration = 5.0f;

	/** Duration in seconds for engine to spool down from current throttle to zero */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spool Configuration", meta = (ClampMin = "0.1", ClampMax = "30.0"))
	float EngineSpoolDownDuration = 8.0f;

	/** Rotor speed value at idle (after rotor spool-up completes) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spool Configuration", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float IdleRotorValue = 0.15f;

	/** Duration in seconds for rotor to spool up to idle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spool Configuration", meta = (ClampMin = "0.1", ClampMax = "30.0"))
	float RotorSpoolUpDuration = 6.0f;

	/** Duration in seconds for rotor to spool down from current speed to zero */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spool Configuration", meta = (ClampMin = "0.1", ClampMax = "30.0"))
	float RotorSpoolDownDuration = 10.0f;

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
