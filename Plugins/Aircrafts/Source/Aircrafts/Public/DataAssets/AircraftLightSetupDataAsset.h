// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "AircraftLightSetupDataAsset.generated.h"

/**
 *
 */
UCLASS()
class AIRCRAFTS_API UAircraftLightSetupDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	UAircraftLightSetupDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Lights)
	bool bUseLights = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Lights)
	bool bHasBeaconLights = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Lights)
	bool bHasNavigationLights = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Lights)
	bool bHasStrobeLights = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	TSoftObjectPtr<class UStaticMesh> LightMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Materials)
	TSoftObjectPtr<class UMaterialInterface> Material_Beacon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Materials)
	TSoftObjectPtr<class UMaterialInterface> Material_Navigation = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Materials)
	TSoftObjectPtr<class UMaterialInterface> Material_Strobe = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Color)
	FLinearColor LightColor_Beacon = FLinearColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Color)
	FLinearColor LightColor_Navigation_Red = FLinearColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Color)
	FLinearColor LightColor_Navigation_Green = FLinearColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Color)
	FLinearColor LightColor_Navigation_White = FLinearColor::White;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Color)
	FLinearColor LightColor_Strobe = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Beacon)
	FName LightMesh_Beacon_1_SocketName = TEXT("Socket_Beacon_1");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Beacon)
	FVector LightMesh_Beacon_1_RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Beacon)
	FName LightMesh_Beacon_2_SocketName = TEXT("Socket_Beacon_2");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Beacon)
	FVector LightMesh_Beacon_2_RelativeLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Navigation)
	FName LightMesh_NavLight_Green_SocketName = TEXT("Socket_NavLight_Green");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Navigation)
	FVector LightMesh_NavLight_Green_RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Navigation)
	FName LightMesh_NavLight_Red_SocketName = TEXT("Socket_NavLight_Red");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Navigation)
	FVector LightMesh_NavLight_Red_RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Navigation)
	FName LightMesh_NavLight_White_SocketName = TEXT("Socket_NavLight_White");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Navigation)
	FVector LightMesh_NavLight_White_RelativeLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Strobe)
	FName LightMesh_Strobe_1_SocketName = TEXT("Socket_Strobe_1");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Strobe)
	FVector LightMesh_Strobe_1_RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Strobe)
	FName LightMesh_Strobe_2_SocketName = TEXT("Socket_Strobe_2");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Strobe)
	FVector LightMesh_Strobe_2_RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Strobe)
	FName LightMesh_Strobe_3_SocketName = TEXT("Socket_Strobe_3");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Strobe)
	FVector LightMesh_Strobe_3_RelativeLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RandomPhase)
	FName RandomPhaseMaterialIdentifier = TEXT("RandomPhase");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RandomPhase)
	float StrobeModifier = 0.5f; // This is a multiplier for the strobe light phase, can be used to adjust the strobe effect
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

		if(LightMesh.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Error,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() called with a null LightMesh.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return AssetObjects;
		}
		else
		{
			AssetObjects.Add(LightMesh);
			UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() added LightMesh: {LightMesh}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("LightMesh", *LightMesh.ToString())
			);
		}

		if (Material_Beacon.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Error,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() called with a null Material_Beacon.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
		}
		else
		{
			AssetObjects.Add(Material_Beacon);
			UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() added Material_Beacon: {MaterialBeacon}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("MaterialBeacon", *Material_Beacon.ToString())
			);
		}

		if (Material_Navigation.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Error,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() called with a null Material_Navigation.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
		}
		else
		{
			AssetObjects.Add(Material_Navigation);
			UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() added Material_Navigation: {MaterialNavigation}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("MaterialNavigation", *Material_Navigation.ToString())
			);
		}

		if (Material_Strobe.IsNull())
		{
			UE_LOGFMT(Log_PreLoadingDataAsset, Error,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() called with a null Material_Strobe.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
		}
		else
		{
			AssetObjects.Add(Material_Strobe);
			UE_LOGFMT(Log_PreLoadingDataAsset, VeryVerbose,
				"{Time}: {Line} {Class}: GetAssetObjects_Implementation() added Material_Strobe: {MaterialStrobe}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("MaterialStrobe", *Material_Strobe.ToString())
			);
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