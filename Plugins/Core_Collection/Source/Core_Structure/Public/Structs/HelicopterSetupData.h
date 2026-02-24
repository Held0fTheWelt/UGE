#pragma once

#include "CoreMinimal.h"
#include "HelicopterSetupData.generated.h"

USTRUCT(BlueprintType)
struct CORE_STRUCTURE_API FHelicopterSetupData
{
	GENERATED_BODY()

public:
	FHelicopterSetupData()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TSoftObjectPtr<class UCurveFloat> PitchCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Acceleration = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Momentum_Max = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Momentum_Min = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Thrust_Max = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Speed_Yaw_Max = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Speed_Pitch_Max = 2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Speed_Roll_Max = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Velocity_Max = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	bool bInvertPitch = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	bool bInvertYaw = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Power_Hover = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Power_Impulse_Up = 50.f; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Power_Impulse_Down = -200.f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float FlipVectorRightLine = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	float Gravity_Force = 500.f;

	/** Yaw angular acceleration scalar (deg/s²). Uses AddTorqueInDegrees with bAccelChange=true (mass-independent). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics|Yaw")
	float TorqueMultiplier = 24.f;
	/** Pitch torque from cyclic (Nm/deg). Applied when HelicopterSetupDataAsset is loaded. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics|Cyclic")
	float CyclicPitchTorqueGain = 50.f;
	/** Roll torque from cyclic (Nm/deg). Applied when HelicopterSetupDataAsset is loaded. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics|Cyclic")
	float CyclicRollTorqueGain = 50.f;
	/** Minimum thrust (N) from cyclic disc tilt at zero collective. 0 = disabled. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics|Cyclic")
	float CyclicIdleThrust = 100000.f;
};
