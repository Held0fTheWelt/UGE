// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "Structs/WheelGroup.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "TransmissionSystem.h"
#include "ChaosVehicleSetupDataAsset.generated.h"



/**
 * 
 */
UCLASS()
class ELEMENTAL_DATA_API UChaosVehicleSetupDataAsset : public UPreLoadingDataAsset
{
	GENERATED_BODY()

public:
	UChaosVehicleSetupDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		AssetTypeName = GetAssetType_Implementation();
		// Default constructor can initialize properties if needed
		bReverseAsBrake = true;
		bParkEnabled = false;
		Mass = 1500.0f;
		ChassisWidth = 180.f;
		ChassisHeight = 140.f;
		DragCoefficient = 0.3f;
		DownforceCoefficient = 0.3f;
		InertiaTensorScale = FVector(1.0f, 1.0f, 1.0f);
		SleepThreshold = 10.0f;
		SleepSlopeLimit = 0.866f;	// 30 degrees, Cos(30)

		TorqueControl.InitDefaults();
		TargetRotationControl.InitDefaults();
		StabilizeControl.InitDefaults();

		bReverseAsBrake = true;
		bParkEnabled = false;
		Mass = 1500.0f;
		ChassisWidth = 180.f;
		ChassisHeight = 140.f;
		DragCoefficient = 0.3f;
		DownforceCoefficient = 0.3f;
		InertiaTensorScale = FVector(1.0f, 1.0f, 1.0f);
		SleepThreshold = 10.0f;
		SleepSlopeLimit = 0.866f;	// 30 degrees, Cos(30)

		bIsTank = false;

		TorqueControl.InitDefaults();
		TargetRotationControl.InitDefaults();
		StabilizeControl.InitDefaults();


		bRequiresControllerForInputs = true;
		IdleBrakeInput = 0.0f;
		StopThreshold = 10.0f;
		WrongDirectionThreshold = 100.f;
		ThrottleInputRate.RiseRate = 6.0f;
		ThrottleInputRate.FallRate = 10.0f;
		ThrottleInputRate.InputCurveFunction = EInputFunctionType::LinearFunction;
		BrakeInputRate.RiseRate = 6.0f;
		BrakeInputRate.FallRate = 10.0f;
		BrakeInputRate.InputCurveFunction = EInputFunctionType::LinearFunction;
		SteeringInputRate.RiseRate = 2.5f;
		SteeringInputRate.FallRate = 5.0f;
		SteeringInputRate.InputCurveFunction = EInputFunctionType::SquaredFunction;
		HandbrakeInputRate.RiseRate = 12.0f;
		HandbrakeInputRate.FallRate = 12.0f;
		PitchInputRate.RiseRate = 6.0f;
		PitchInputRate.FallRate = 10.0f;
		PitchInputRate.InputCurveFunction = EInputFunctionType::LinearFunction;
		RollInputRate.RiseRate = 6.0f;
		RollInputRate.FallRate = 10.0f;
		RollInputRate.InputCurveFunction = EInputFunctionType::LinearFunction;
		YawInputRate.RiseRate = 6.0f;
		YawInputRate.FallRate = 10.0f;
		YawInputRate.InputCurveFunction = EInputFunctionType::LinearFunction;
		TransmissionType = Chaos::ETransmissionType::Automatic;
	}

	UPROPERTY(EditAnywhere, Category = WheelSetup)
	bool bSuspensionEnabled;

	UPROPERTY(EditAnywhere, Category = WheelSetup)
	bool bWheelFrictionEnabled;

	UPROPERTY(EditAnywhere, Category = WheelSetup)
	bool bLegacyWheelFrictionPosition;

	/** Wheels to create */
	UPROPERTY(EditAnywhere, Category = WheelSetup)
	TArray<FChaosWheelSetup> WheelSetups;

	UPROPERTY(EditAnywhere, Category = WheelSetup, meta = (EditCondition = "bMechanicalSimEnabled"))
	bool bIsTank;

	UPROPERTY(EditAnywhere, Category = WheelSetup)
	TArray<FWheelGroup> WheelGroups;

	UPROPERTY(EditAnywhere, Category = Custom)
	struct FCollisionResponseContainer WheelTraceCollisionResponses;

	UPROPERTY(EditAnywhere, Category = MechanicalSetup)
	bool bMechanicalSimEnabled;

	/** Engine */
	UPROPERTY(EditAnywhere, Category = MechanicalSetup, meta = (EditCondition = "bMechanicalSimEnabled"))
	FVehicleEngineConfig EngineSetup;

	/** Differential */
	UPROPERTY(EditAnywhere, Category = MechanicalSetup, meta = (EditCondition = "bMechanicalSimEnabled"))
	FVehicleDifferentialConfig DifferentialSetup;

	/** Transmission data */
	UPROPERTY(EditAnywhere, Category = MechanicalSetup, meta = (EditCondition = "bMechanicalSimEnabled"))
	FVehicleTransmissionConfig TransmissionSetup;

	/** Transmission data */
	UPROPERTY(EditAnywhere, Category = SteeringSetup)
	FVehicleSteeringConfig SteeringSetup;

	// Our instanced wheels
	UPROPERTY(transient, duplicatetransient, BlueprintReadOnly, Category = Vehicle)
	TArray<TObjectPtr<class UChaosVehicleWheel>> Wheels;

	UPROPERTY(EditAnywhere, Category = VehicleSetup)
	bool bReverseAsBrake;
	UPROPERTY(EditAnywhere, Category = VehicleSetup)
	bool bThrottleAsBrake;
	UPROPERTY(EditAnywhere, Category = VehicleSetup)
	bool bParkEnabled;

	/** Mass to set the vehicle chassis to. It's much easier to tweak vehicle settings when
	 * the mass doesn't change due to tweaks with the physics asset. [kg] */
	UPROPERTY(EditAnywhere, Category = VehicleSetup, meta = (ClampMin = "0.01", UIMin = "0.01"))
	float Mass;

	/**
	 * Enable to override the calculated COM position with your own fixed value - this prevents the vehicle handling changing when the asset changes
	 */
	UPROPERTY(EditAnywhere, Category = VehicleSetup)
	bool bEnableCenterOfMassOverride;

	/**
	 * The center of mass override value, this value overrides the calculated COM and the COM offset value in the mesh is also ignored.
	 */
	UPROPERTY(EditAnywhere, Category = VehicleSetup, meta = (EditCondition = "bEnableCenterOfMassOverride"))
	FVector CenterOfMassOverride;

	/** Chassis width used for drag force computation (cm)*/
	UPROPERTY(EditAnywhere, Category = VehicleSetup, meta = (ClampMin = "0.01", UIMin = "0.01"))
	float ChassisWidth;

	/** Chassis height used for drag force computation (cm)*/
	UPROPERTY(EditAnywhere, Category = VehicleSetup, meta = (ClampMin = "0.01", UIMin = "0.01"))
	float ChassisHeight;

	/** DragCoefficient of the vehicle chassis - force resisting forward motion at speed */
	UPROPERTY(EditAnywhere, Category = VehicleSetup)
	float DragCoefficient;

	/** DownforceCoefficient of the vehicle chassis - force pressing vehicle into ground at speed */
	UPROPERTY(EditAnywhere, Category = VehicleSetup)
	float DownforceCoefficient;

	// Drag area in Meters^2
	UPROPERTY(transient)
	float DragArea;

	// Debug drag magnitude last applied
	UPROPERTY(transient)
	float DebugDragMagnitude;

	/** Scales the vehicle's inertia in each direction (forward, right, up) */
	UPROPERTY(EditAnywhere, Category = VehicleSetup, AdvancedDisplay)
	FVector InertiaTensorScale;

	/** Option to apply some aggressive sleep logic, larger number is more agressive, 0 disables */
	UPROPERTY(EditAnywhere, Category = VehicleSetup)
	float SleepThreshold;

	/** Option to apply some aggressive sleep logic if slopes up Z is less than this value, i.e value = Cos(SlopeAngle) so 0.866 will sleep up to 30 degree slopes */
	UPROPERTY(EditAnywhere, Category = VehicleSetup, meta = (ClampMin = "0.01", UIMin = "0.01", ClampMax = "1.0", UIMax = "1.0"))
	float SleepSlopeLimit;

	/** Optional aerofoil setup - can be used for car spoilers or aircraft wings/elevator/rudder */
	UPROPERTY(EditAnywhere, Category = AerofoilSetup)
	TArray<FVehicleAerofoilConfig> Aerofoils;

	/** Optional thruster setup, use one or more as your main engine or as supplementary booster */
	UPROPERTY(EditAnywhere, Category = ThrusterSetup)
	TArray<FVehicleThrustConfig> Thrusters;

	/** Arcade style direct control of vehicle rotation via torque force */
	UPROPERTY(EditAnywhere, Category = ArcadeControl)
	FVehicleTorqueControlConfig TorqueControl;

	/** Arcade style direct control of vehicle rotation via torque force */
	UPROPERTY(EditAnywhere, Category = ArcadeControl)
	FVehicleTargetRotationControlConfig TargetRotationControl;

	/** Arcade style control of vehicle */
	UPROPERTY(EditAnywhere, Category = ArcadeControl)
	FVehicleStabilizeControlConfig StabilizeControl;

	// Bypass the need for a controller in order for the controls to be processed.
	UPROPERTY(EditAnywhere, Category = VehicleInput)
	bool bRequiresControllerForInputs;

	// How much to press the brake when the player has release throttle
	UPROPERTY(EditAnywhere, Category = VehicleInput)
	float IdleBrakeInput;

	// Auto-brake when absolute vehicle forward speed is less than this (cm/s)
	UPROPERTY(EditAnywhere, Category = VehicleInput)
	float StopThreshold;

	// Auto-brake when vehicle forward speed is opposite of player input by at least this much (cm/s)
	UPROPERTY(EditAnywhere, Category = VehicleInput)
	float WrongDirectionThreshold;

public:
	// Rate at which input throttle can rise and fall
	UPROPERTY(EditAnywhere, Category = VehicleInput, AdvancedDisplay)
	FVehicleInputRateConfig ThrottleInputRate;

	// Rate at which input brake can rise and fall
	UPROPERTY(EditAnywhere, Category = VehicleInput, AdvancedDisplay)
	FVehicleInputRateConfig BrakeInputRate;

	// Rate at which input steering can rise and fall
	UPROPERTY(EditAnywhere, Category = VehicleInput, AdvancedDisplay)
	FVehicleInputRateConfig SteeringInputRate;

	// Rate at which input handbrake can rise and fall
	UPROPERTY(EditAnywhere, Category = VehicleInput, AdvancedDisplay)
	FVehicleInputRateConfig HandbrakeInputRate;

	// Rate at which input pitch can rise and fall
	UPROPERTY(EditAnywhere, Category = VehicleInput, AdvancedDisplay)
	FVehicleInputRateConfig PitchInputRate;

	// Rate at which input roll can rise and fall
	UPROPERTY(EditAnywhere, Category = VehicleInput, AdvancedDisplay)
	FVehicleInputRateConfig RollInputRate;

	// Rate at which input yaw can rise and fall
	UPROPERTY(EditAnywhere, Category = VehicleInput, AdvancedDisplay)
	FVehicleInputRateConfig YawInputRate;

	//UPROPERTY(EditAnywhere, Category = ArcadeControl)
	TEnumAsByte<Chaos::ETransmissionType> TransmissionType;

	virtual const FPrimaryAssetType GetAssetType_Implementation() const override
	{
		return AssetType;
	}

	static FPrimaryAssetType GetAssetType()
	{
		return AssetType;
	}

private:
	static const FPrimaryAssetType AssetType;
};
