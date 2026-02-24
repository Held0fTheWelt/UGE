#pragma once

#include "CoreMinimal.h"
#include "Components/LightComponentBase.h"
#include "Enums/VehicleLightType.h"
#include "Enums/LightType.h"
#include "Structs/CollisionSetup.h"
#include "VehicleLight.generated.h"

USTRUCT(BlueprintType)
struct CORE_STRUCTURE_API FVehicleLight
{
	GENERATED_BODY()

public:
	FVehicleLight(EVehicleLightType VehicleLightType, ELightType LightType):
		VehicleLightType(VehicleLightType),
		LightTransform(FTransform::Identity),
		LightColor(FLinearColor::White),
		Intensity(1.0f),
		IntensityUnits(ELightUnits::Lumens),
		AttenuationRadius(1000.0f),
		InnerConeAngle(30.0f),
		OuterConeAngle(45.0f),
		IESTexture(nullptr),
		LightType(LightType)
	{
	}

	FVehicleLight() :
		VehicleLightType(EVehicleLightType::VLT_NONE),
		LightTransform(FTransform::Identity),
		LightColor(FLinearColor::White),
		Intensity(1.0f),
		IntensityUnits(ELightUnits::Lumens),
		AttenuationRadius(1000.0f),
		InnerConeAngle(30.0f),
		OuterConeAngle(45.0f),
		IESTexture(nullptr),
		LightType(ELightType::LT_NONE)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	EVehicleLightType VehicleLightType = EVehicleLightType::VLT_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	FTransform LightTransform = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	FLinearColor LightColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	float Intensity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Light, meta = (DisplayName = "Intensity Units", EditCondition = "!bUseIESBrightness"))
	ELightUnits IntensityUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	float AttenuationRadius = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	float InnerConeAngle = 30.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	float OuterConeAngle = 45.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	TSoftObjectPtr<class UTextureLightProfile> IESTexture;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	ELightType LightType = ELightType::LT_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	FCollisionSetup CollisionSetup;
};
