
#pragma once

#include "ChaosWheeledVehicleMovementComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "DataAssets/ChaosVehicleSetupDataAsset.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY_STATIC(Log_VehicleEngineAssignmentDataHelper, Log, All);

class CORE_CLASSES_API FEngineAssignmentDataHelper
{
public:
	// Beispiel: Template-Methode zur ?bertragung von Daten
	static void ApplyData(const UChaosVehicleSetupDataAsset* Asset, UChaosWheeledVehicleMovementComponent* TargetComponent)
	{
		if (!Asset || !TargetComponent)
		{
			if(!Asset)
			{
				UE_LOGFMT(Log_VehicleEngineAssignmentDataHelper, Error, "{Time}: {Line} {Class}: Asset was null.",
					("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
			}
			if (!TargetComponent)
			{
				UE_LOGFMT(Log_VehicleEngineAssignmentDataHelper, Error, "{Time}: {Line} {Class}: TargetComponent was null.",
					("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
			}
			return;
		}

		TargetComponent->bSuspensionEnabled = Asset->bSuspensionEnabled;
		TargetComponent->bWheelFrictionEnabled = Asset->bWheelFrictionEnabled;
		TargetComponent->bLegacyWheelFrictionPosition = Asset->bLegacyWheelFrictionPosition;
		TargetComponent->WheelSetups = Asset->WheelSetups;
		TargetComponent->WheelTraceCollisionResponses = Asset->WheelTraceCollisionResponses;
		TargetComponent->bMechanicalSimEnabled = Asset->bMechanicalSimEnabled;
		TargetComponent->EngineSetup = Asset->EngineSetup;
		TargetComponent->DifferentialSetup = Asset->DifferentialSetup;
		TargetComponent->TransmissionSetup = Asset->TransmissionSetup;
		TargetComponent->SteeringSetup = Asset->SteeringSetup;
		TargetComponent->Wheels = Asset->Wheels;
		TargetComponent->bReverseAsBrake = Asset->bReverseAsBrake;
		TargetComponent->bThrottleAsBrake = Asset->bThrottleAsBrake;
		// Parkenabled ???
		TargetComponent->Mass = Asset->Mass;
		TargetComponent->bEnableCenterOfMassOverride = Asset->bEnableCenterOfMassOverride;
		TargetComponent->CenterOfMassOverride = Asset->CenterOfMassOverride;
		TargetComponent->ChassisWidth = Asset->ChassisWidth;
		TargetComponent->ChassisHeight = Asset->ChassisHeight;
		TargetComponent->DragCoefficient = Asset->DragCoefficient;
		TargetComponent->DownforceCoefficient = Asset->DownforceCoefficient;
		TargetComponent->DragArea = Asset->DragArea;
		TargetComponent->DebugDragMagnitude = Asset->DebugDragMagnitude;
		TargetComponent->InertiaTensorScale = Asset->InertiaTensorScale;
		TargetComponent->SleepThreshold = Asset->SleepThreshold;
		TargetComponent->SleepSlopeLimit = Asset->SleepSlopeLimit;
		TargetComponent->Aerofoils = Asset->Aerofoils;
		TargetComponent->Thrusters = Asset->Thrusters;
		TargetComponent->TorqueControl = Asset->TorqueControl;
		TargetComponent->TargetRotationControl = Asset->TargetRotationControl;
		TargetComponent->StabilizeControl = Asset->StabilizeControl;
		TargetComponent->SetRequiresControllerForInputs(Asset->bRequiresControllerForInputs);
		//TargetComponent->SetIdleBrakeInput = Asset->IdleBrakeInput;
		//TargetComponent->StopThreshold = Asset->StopThreshold;
		//TargetComponent->WrongDirectionThreshold = Asset->WrongDirectionThreshold;
		TargetComponent->ThrottleInputRate = Asset->ThrottleInputRate;
		TargetComponent->BrakeInputRate = Asset->BrakeInputRate;
		TargetComponent->SteeringInputRate = Asset->SteeringInputRate;
		TargetComponent->HandbrakeInputRate = Asset->HandbrakeInputRate;
		TargetComponent->PitchInputRate = Asset->PitchInputRate;
		TargetComponent->RollInputRate = Asset->RollInputRate;
		TargetComponent->YawInputRate = Asset->YawInputRate;
		//TargetComponent->TransmissionType = Asset->TransmissionType;

	}
};
