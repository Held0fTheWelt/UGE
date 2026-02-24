// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Enums/HelicopterPhysicalComponentType.h"
#include "Structs/HelicopterPhysicsBone.h"
#include "PhysicsBasedHelicopterDefinitionDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class HELICOPTERS_API UPhysicsBasedHelicopterDefinitionDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()
	
public:
	UPhysicsBasedHelicopterDefinitionDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics")
	FHelicopterPhysicsBone Body = FHelicopterPhysicsBone(EHelicopterPhysicalComponentType::HPT_BODY);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics")
	FHelicopterPhysicsBone MainRotorBone = FHelicopterPhysicsBone(EHelicopterPhysicalComponentType::HPT_ROTOR_SHAFT);;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics")
	FHelicopterPhysicsBone TailRotorBone = FHelicopterPhysicsBone(EHelicopterPhysicalComponentType::HPT_ROTOR_TAIL);;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics")
	TArray<FName> MainRotorBoneNames;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics")
	TArray<FName> TailRotorBoneNames;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics")
	TArray<FName> ClocheBoneNames;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics")
	TArray<FName> AileronBoneNames;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics")
	FName BoneName_Rotor_Angle_X;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics")
	FName BoneName_Rotor_Angle_Y;

	/** Yaw angular acceleration scalar (deg/s²). Uses AddTorqueInDegrees with bAccelChange=true (mass-independent). With RotorTail~5: 24 → ~120 deg/s². */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Torque")
	float TorqueMultiplier = 24.f;

	/** Maximum angular velocity in deg/s. Prevents runaway rotation from cyclic force moments. 0 = UE default (no custom limit). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Stability")
	float MaxAngularVelocityDeg = 90.f;

	/** Explicit pitch torque from cyclic disc tilt (Nm per degree of tilt). Lower = less wobble. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Torque")
	float CyclicPitchTorqueGain = 50.f;

	/** Explicit roll torque from cyclic disc tilt (Nm per degree of tilt). Lower = less wobble. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Torque")
	float CyclicRollTorqueGain = 50.f;

	/** Minimum thrust (N) when disc is tilted via cyclic. 0 = disabled. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Torque")
	float CyclicIdleThrust = 100000.f;

	/** Degrees per second per unit rotor speed (0..1); visual rotation. ~-360 = 1 rev/s at full throttle. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Visual")
	float Rotor_Rotation_Scalar = -360.f;

	/** Calibration: Invert roll response (if cyclic X produces wrong roll direction) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Calibration")
	bool bInvertRoll = false;

	/** Calibration: Invert pitch response (if cyclic Y produces wrong pitch direction) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Calibration")
	bool bInvertPitch = false;

	/** Calibration: Swap cyclic X/Y mapping (if forward/back and left/right are swapped) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Calibration")
	bool bSwapCyclicXY = false;

	/** Calibration: Invert yaw response (if pedals produce wrong yaw direction) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Calibration")
	bool bInvertYaw = false;

	/** Optional: Override physics body mass in kg (if not using mesh mass). Leave at 0 to use mesh mass. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Mass", meta=(ClampMin="0"))
	float OverrideMass = 0.f;

	/** Optional: Main rotor diameter in cm (for reference/documentation). Not used in physics. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Specifications")
	float MainRotorDiameter_cm = 0.f;

	/** Optional: Tail rotor diameter in cm (for reference/documentation). Not used in physics. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Specifications")
	float TailRotorDiameter_cm = 0.f;

	/** Optional: Helicopter length in cm (for reference/documentation). Not used in physics. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Helicopter Physics|Specifications")
	float Length_cm = 0.f;

	/** Returns the static primary asset type for this data asset class */
	virtual const FPrimaryAssetType GetAssetType_Implementation() const override
	{
		return AssetType;
	}

	static FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}

private:
	/** The static asset type for this data asset class */
	static const FPrimaryAssetType AssetType;
};
