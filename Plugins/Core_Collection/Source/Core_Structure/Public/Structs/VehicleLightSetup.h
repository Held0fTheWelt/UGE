#pragma once

#include "CoreMinimal.h"
#include "Structs/VehicleLight.h"
#include "Enums/LightType.h"
#include "VehicleLightSetup.generated.h"

USTRUCT(BlueprintType)
struct CORE_STRUCTURE_API FVehicleLightSetup 
{
	GENERATED_BODY()

public:
	FVehicleLightSetup()
	{
		VehicleLights.Add(FVehicleLight(EVehicleLightType::VLT_BLINK_FRONT, ELightType::LT_LIGHT_SPOT));
		VehicleLights.Add(FVehicleLight(EVehicleLightType::VLT_FRONT, ELightType::LT_LIGHT_SPOT));
		VehicleLights.Add(FVehicleLight(EVehicleLightType::VLT_FRONT, ELightType::LT_LIGHT_SPOT));
		VehicleLights.Add(FVehicleLight(EVehicleLightType::VLT_BLINK_FRONT, ELightType::LT_LIGHT_SPOT));
		VehicleLights.Add(FVehicleLight(EVehicleLightType::VLT_BLINK_REAR, ELightType::LT_LIGHT_POINT)); 
		VehicleLights.Add(FVehicleLight(EVehicleLightType::VLT_REAR, ELightType::LT_LIGHT_POINT));
		VehicleLights.Add(FVehicleLight(EVehicleLightType::VLT_REAR, ELightType::LT_LIGHT_POINT));
		VehicleLights.Add(FVehicleLight(EVehicleLightType::VLT_BLINK_REAR, ELightType::LT_LIGHT_POINT));
		BrakeLightMaterialIndices = TArray<int32>();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Lights")
	TArray<FVehicleLight> VehicleLights; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Light Materials")
	TArray<int32> BrakeLightMaterialIndices;
};
