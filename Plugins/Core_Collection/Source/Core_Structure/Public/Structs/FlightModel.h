#pragma once

#include "CoreMinimal.h"
#include "Enums/DiplomaticStatus.h"
#include "Engine/DataTable.h"
#include "FlightModel.generated.h"

USTRUCT(BlueprintType)
struct CORE_STRUCTURE_API FFlightModelType
{
	GENERATED_BODY()

public:
	FFlightModelType()
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName Type;
	/** Empty Weight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float EmptyWeight = 0.f;
	/** Max Weight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float MaxWeight = 0.f;

	/** Max Velocity in km/h needs to be converted to m/s */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Vmax = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float MainRotorDiameter = 0.f;
};

USTRUCT(BlueprintType)
struct CORE_STRUCTURE_API FFlightModel : public FTableRowBase
{
	GENERATED_BODY()

public:
	FFlightModel()
	{
		Name = EName::None;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
	FName NameIdentifier;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = General)
	TArray<FFlightModelType> Types;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Hover")
	float HoveringForce = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Hover")
	float HoveringTolerance = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Hover")
	float CollectiveInputMin = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Hover")
	float CollectiveInputMax = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Hover")
	float MaxCeilingHeight = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Hover")
	float MaxCeilingHeightClampMin = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Hover")
	float MaxCeilingHeightClampMax = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Engine")
	float EngineThrust = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Engine")
	float TurboThrust = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Engine")
	float MaxFlightSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Damping")
	float LinearDampingParkBrakeOn = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Damping")
	float LinearDampingParkBrakeOff = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Damping")
	float LinearDampingDuringFlight = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config | Damping")
	float ParkingBrakesAltitudeActivationHeight = 0.f;


};
