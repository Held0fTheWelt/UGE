/*
 * Copyright (c) 2024 Yves Tanas
 * All Rights Reserved.
 *
 * This file is part of the Collections project.
 *
 * Unauthorized copying of this file, via any medium, is strictly prohibited.
 * Proprietary and confidential.
 *
 * This software may be used only as expressly authorized by the copyright holder.
 * Unless required by applicable law or agreed to in writing, software distributed
 * under this license is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 *
 * For licensing inquiries, please contact: yves.tanas@example.com
 *
 * Contributors:
 *   Yves Tanas <yves.tanas@example.com>
 *
 * -------------------------------------------------------------------------------
 * File:        [UnifiedPlayerController.cpp]
 * Created:     [2025-06-12]
 * Description: Implementation for player input control switching and interface dispatch.
 * -------------------------------------------------------------------------------
 */

#include "Controllers/Player/UnifiedPlayerController.h"

#include "Interfaces/Control/CameraControlInterface.h"
#include "Interfaces/Control/EngineControlInterface.h"
#include "Interfaces/Control/EngineInputControlInterface.h"
#include "Interfaces/Control/HelicopterControlInterface.h"
#include "Interfaces/Control/HumanoidControlInterface.h"
#include "Interfaces/Control/InteractControlInterface.h"
#include "Interfaces/Control/TurretControlInterface.h"
#include "Interfaces/Control/PawnControlInterface.h"
#include "Interfaces/Control/VehicleControlInterface.h"

#include "Interfaces/PhysicalSenseInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Logging/Log_Controllers.h"

#pragma region "Turret Control"
void AUnifiedPlayerController::Rotate(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UTurretControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: CameraControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	FVector2D LookRotation = Value.Get<FVector2D>();

	ITurretControlInterface::Execute_Rotate(InterfaceComponent, LookRotation);
}
#pragma endregion "Turret Control"

#pragma region "Engine Control"
void AUnifiedPlayerController::StartEngineAction()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UEngineControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IEngineControlInterface::Execute_StartEngineSwitch(InterfaceComponent);
}

void AUnifiedPlayerController::StopEngineAction()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UEngineControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IEngineControlInterface::Execute_StopEngineSwitch(InterfaceComponent);
}

void AUnifiedPlayerController::StartSecondEngineAction()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UEngineControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IEngineControlInterface::Execute_StartSecondEngineSwitch(InterfaceComponent);
}

void AUnifiedPlayerController::StopSecondEngineAction()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UEngineControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IEngineControlInterface::Execute_StopSecondEngineSwitch(InterfaceComponent);
}
#pragma endregion "Engine Control"

#pragma region "Systems Control"
void AUnifiedPlayerController::ToggleSASAction()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HelicopterControlInterface is not registered, SAS toggle ignored",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_SASToggle(InterfaceComponent);
}
#pragma endregion "Systems Control"

#pragma region "Helicopter Input Control"
void AUnifiedPlayerController::Cyclic(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}
	FVector2D CyclicVector = Value.Get<FVector2D>();

	IHelicopterControlInterface::Execute_CyclicInput(InterfaceComponent, CyclicVector);
}

void AUnifiedPlayerController::CyclicCompleted(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_CyclicInput(InterfaceComponent, FVector2D::Zero());
}

void AUnifiedPlayerController::CyclicKeyWStart()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_CyclicInputKey(InterfaceComponent, 0, 1.f);
}

void AUnifiedPlayerController::CyclicKeyWStop()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_CyclicInputKey(InterfaceComponent, 0, 0.f);
}

void AUnifiedPlayerController::CyclicKeySStart()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_CyclicInputKey(InterfaceComponent, 1, 1.f);
}

void AUnifiedPlayerController::CyclicKeySStop()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_CyclicInputKey(InterfaceComponent, 1, 0.f);
}

void AUnifiedPlayerController::CyclicKeyAStart()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_CyclicInputKey(InterfaceComponent, 2, 1.f);
}

void AUnifiedPlayerController::CyclicKeyAStop()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_CyclicInputKey(InterfaceComponent, 2, 0.f);
}

void AUnifiedPlayerController::CyclicKeyDStart()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_CyclicInputKey(InterfaceComponent, 3, 1.f);
}

void AUnifiedPlayerController::CyclicKeyDStop()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_CyclicInputKey(InterfaceComponent, 3, 0.f);
}

void AUnifiedPlayerController::Collective(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HelicopterControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}
	float CollectiveValue = Value.Get<float>();
	IHelicopterControlInterface::Execute_CollectiveInput(InterfaceComponent, CollectiveValue);
}

void AUnifiedPlayerController::CollectiveKeyStart()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HelicopterControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_CollectiveInput(InterfaceComponent, true);
}

void AUnifiedPlayerController::CollectiveKeyStop()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HelicopterControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_CollectiveInput(InterfaceComponent, false);
}

void AUnifiedPlayerController::Pedals(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HelicopterControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	float PedalsValue = Value.Get<float>();
	IHelicopterControlInterface::Execute_PedalsInput(InterfaceComponent, PedalsValue);
}

void AUnifiedPlayerController::PedalsKeyLeftStart()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HelicopterControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_PedalsInputsKey(InterfaceComponent, 0, true);
}

void AUnifiedPlayerController::PedalsKeyLeftStop()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HelicopterControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_PedalsInputsKey(InterfaceComponent, 0, false);
}

void AUnifiedPlayerController::PedalsKeyRightStart()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HelicopterControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_PedalsInputsKey(InterfaceComponent, 1, true);
}

void AUnifiedPlayerController::PedalsKeyRightStop()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHelicopterControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HelicopterControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHelicopterControlInterface::Execute_PedalsInputsKey(InterfaceComponent, 1, false);
}
#pragma endregion "Helicopter Input Control"

#pragma region "Humanoid Control"
void AUnifiedPlayerController::Jump()
{
	UE_LOGFMT(Log_UnifiedController_Controls, Verbose, "{Time}: {Line} {Class}: Jump action triggered.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHumanoidControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HumanoidsInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}


	IHumanoidControlInterface::Execute_Jump(InterfaceComponent);
}

void AUnifiedPlayerController::StopJumping()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHumanoidControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HumanoidsInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHumanoidControlInterface::Execute_StopJumping(InterfaceComponent);
}

void AUnifiedPlayerController::Run()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHumanoidControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HumanoidsInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHumanoidControlInterface::Execute_Run(InterfaceComponent);
}

void AUnifiedPlayerController::StopRunning()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHumanoidControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HumanoidsInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IHumanoidControlInterface::Execute_StopRunning(InterfaceComponent);
}

void AUnifiedPlayerController::Move(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UHumanoidControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: HumanoidsInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	FVector2D MoveVector = Value.Get<FVector2D>();
	UE_LOGFMT(Log_UnifiedController_Controls, Verbose, "{Time}: {Line} {Class}: Current MoveVector: {MoveX} {MoveY}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("MoveX", MoveVector.X), ("MoveY", MoveVector.Y));

	IHumanoidControlInterface::Execute_Move(InterfaceComponent, MoveVector);
}
#pragma endregion "Humanoid Control"

#pragma region "Vehicle Control"
void AUnifiedPlayerController::Steering(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UVehicleControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: VehiclesInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	float SteeringValue = Value.Get<float>();
	UE_LOGFMT(Log_UnifiedController_Controls, Verbose, "{Time}: {Line} {Class}: Current SteeringValue: {SteeringValue}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("SteeringValue", SteeringValue));
	IVehicleControlInterface::Execute_Steering(InterfaceComponent, SteeringValue);
}

void AUnifiedPlayerController::SteeringCompleted(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UVehicleControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: VehiclesInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	float SteeringValue = Value.Get<float>();

	IVehicleControlInterface::Execute_StopSteering(InterfaceComponent);
}

void AUnifiedPlayerController::Brake(const FInputActionValue& Value)
{
	float BrakeValue = Value.Get<float>();
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UVehicleControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: VehiclesInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IVehicleControlInterface::Execute_Brake(InterfaceComponent, BrakeValue);
}

void AUnifiedPlayerController::StartBrake(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UVehicleControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: VehiclesInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IVehicleControlInterface::Execute_StartBrake(InterfaceComponent);
}

void AUnifiedPlayerController::StopBrake(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UVehicleControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: VehiclesInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IVehicleControlInterface::Execute_StopBrake(InterfaceComponent);
}

void AUnifiedPlayerController::StartHandbrake(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UVehicleControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: VehiclesInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IVehicleControlInterface::Execute_StartHandbrake(InterfaceComponent);
}

void AUnifiedPlayerController::StopHandbrake(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UVehicleControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: VehiclesInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}
	IVehicleControlInterface::Execute_StopHandbrake(InterfaceComponent);
}

void AUnifiedPlayerController::ResetVehicle(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UVehicleControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: VehiclesInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	IVehicleControlInterface::Execute_ResetVehicle(InterfaceComponent);
}
#pragma endregion "Vehicle Control"

#pragma region "Camera Control"
/**
 * Handles "look" input (camera or orientation adjustment).
 *
 * The method will try to dispatch the input to the correct interface depending on
 * current registered components and the current control mode (humanoid, vehicle, etc.).
 * Fallbacks ensure that if the controller interface becomes invalid, it tries to reacquire it.
 */

void AUnifiedPlayerController::Look(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UCameraControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: CameraControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	FVector2D LookRotation = Value.Get<FVector2D>();

	UE_LOGFMT(Log_UnifiedController_Controls, Verbose, "{Time}: {Line} {Class}: Look input received: {LookX} {LookY}",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("LookX", LookRotation.X), ("LookY", LookRotation.Y));

	ICameraControlInterface::Execute_Look(InterfaceComponent, LookRotation);

	//FString LookRotStr = LookRotation.ToString();  // z.B. "Pitch=0 Yaw=45 Roll=0"

	//UE_LOGFMT(Log_UnifiedController_Input_Humanoid, Warning,
	//	"{Time}: {Line} {Class}: Look input received: {LookRotation}",
	//	("Class", GET_CLASSNAME_WITH_FUNCTION),
	//	("Line", GET_LINE_NUMBER),
	//	("Time", GET_CURRENT_TIME),
	//	("LookRotation", *LookRotStr)  // *FString → const TCHAR*
	//);

	//if(RegisteredComponentsByClass.Contains())

}

/**
 * Handles camera toggling input (for switching between camera modes/perspectives).
 *
 * Dispatches the toggle action to the current humanoid or vehicle interface, depending on the active control type.
 */
void AUnifiedPlayerController::ToggleCamera(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UCameraControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: CameraControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	FVector2D LookRotation = Value.Get<FVector2D>();
	ICameraControlInterface::Execute_ToggleCamera(InterfaceComponent, true); // Forward toggle

	if(RegisteredComponentsByClass.Contains(UTurretControlInterface::StaticClass()))
	{
		Rotate(LookRotation); // If turret control is available, apply rotation
	}
	//UE_LOGFMT(Log_UnifiedController_Input_All, VeryVerbose, "{Time}: {Line} {Class}: Look input received: {LookRotation}",
	//	("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("LookRotation", LookRotation));
}

void AUnifiedPlayerController::ZoomCamera(const FInputActionValue& Value)
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UCameraControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: CameraControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	ICameraControlInterface::Execute_ZoomCamera(InterfaceComponent, Value.Get<float>()); // zoom
}
#pragma endregion "Camera Control"

#pragma region "Engine Input Control"
void AUnifiedPlayerController::Throttle(const FInputActionValue& Value)
{
	float Throttle = Value.Get<float>();

	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UEngineInputControlInterface::StaticClass());

	if (!InterfaceComponent)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, VeryVerbose, "{Time}: {Line} {Class}: EngineInputControlInterface is not registered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_UnifiedController_Controls, Verbose, "{Time}: {Line} {Class}: Throttle input received: {Throttle}",
		("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("Throttle", Throttle));


	IEngineInputControlInterface::Execute_Throttle(InterfaceComponent, Throttle);
}
#pragma endregion "Engine Input Control"

#pragma region Interact
bool AUnifiedPlayerController::CanInteractWithComponent(UActorComponent* Component) const
{
	APawn* ControlledPawn = GetPawn();

	if (!ControlledPawn || !Component)
	{
		// Log an error if the controlled pawn or component is null
		// This is a critical error, as interaction requires both a pawn and a component.
		if (!ControlledPawn)
		{
			UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: Cannot interact with component. ControlledPawn is null.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return false;
		}

		if (!Component)
		{
			UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: Cannot interact with component. Component is null.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return false;
		}
	}

	// Exit-vehicle case: the registered component belongs to the controlled pawn (we are inside the vehicle).
	// Return true so we call Execute_Interact (same path as Blackhawk). That triggers the component's
	// Interact delegate; the vehicle's listener then calls InteractedWithActor and exit works.
	// (If we returned false we would call InteractOnPawn, which Cobra/others may not implement.)
	if (Component->GetOwner() == ControlledPawn)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: CanInteractWithComponent: ComponentOwner==ControlledPawn (exit vehicle) -> return true (use Execute_Interact path).",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return true;
	}

	TArray<UActorComponent*> SensingComponents = ControlledPawn->GetComponentsByInterface(UPhysicalSenseInterface::StaticClass());

	if (SensingComponents.Num() == 0)
	{
		UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: No sensing components found on controlled pawn.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return false;
	}

	if (!IsValid(SensingComponents[0]))
	{
		UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: Sensing component is not valid.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return false;
	}

	const bool bCanTouch = IPhysicalSenseInterface::Execute_CanTouchActor(SensingComponents[0], Component->GetOwner());
	UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: CanInteractWithComponent: ComponentOwner={ComponentOwner} ControlledPawn={ControlledPawn} -> CanTouch={Result} (false => will try InteractOnPawn/exit).",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("ComponentOwner", Component && Component->GetOwner() ? Component->GetOwner()->GetFName() : NAME_None),
		("ControlledPawn", ControlledPawn ? ControlledPawn->GetFName() : NAME_None),
		("Result", bCanTouch));
	return bCanTouch;
}

/**
 * Handles input for interacting with objects (e.g., "Use" or "Activate" button).
 *
 * Will attempt to use the registered interact controller interface. If the interface is not valid, it will try to reacquire.
 */
void AUnifiedPlayerController::Interact()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UInteractControlInterface::StaticClass());

	// If no registered component (e.g. lost on possess when player left overlap), try to interact with controlled pawn (exit vehicle).
	APawn* ControlledPawn = GetPawn();
	if ((!InterfaceComponent || !InterfaceComponent->GetClass()->ImplementsInterface(UInteractControlInterface::StaticClass())) && ControlledPawn && ControlledPawn->GetClass()->ImplementsInterface(UPawnControlInterface::StaticClass()))
	{
		UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: [EXIT] No registered interact component -> calling InteractOnPawn on {Pawn} (exit vehicle).",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Pawn", ControlledPawn->GetFName()));
		IPawnControlInterface::Execute_InteractOnPawn(ControlledPawn, this);
		return;
	}

	if (!InterfaceComponent || !InterfaceComponent->GetClass()->ImplementsInterface(UInteractControlInterface::StaticClass()))
	{
		UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: [EXIT BLOCKED] No valid InteractControllerInterface and cannot call InteractOnPawn (no pawn or pawn has no PawnControlInterface).",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (CanInteractWithComponent(InterfaceComponent))
	{
		UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: [INTERACT] CanTouch=true -> Execute_Interact on component {ComponentName} (no exit).",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("ComponentName", *InterfaceComponent->GetName()));

		IInteractControlInterface::Execute_Interact(InterfaceComponent, this);
	}
	else
	{
		// We have no colliding interact component here, so try to interact on the controlled pawn (exit).
		ControlledPawn = GetPawn();
		if (!ControlledPawn)
		{
			UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: [EXIT BLOCKED] CanTouch=false but ControlledPawn is null – exit not performed.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		if (ControlledPawn->GetClass()->ImplementsInterface(UPawnControlInterface::StaticClass()))
		{
			UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: [EXIT] CanTouch=false -> calling InteractOnPawn on {Pawn} (exit vehicle).",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Pawn", ControlledPawn->GetFName()));
			IPawnControlInterface::Execute_InteractOnPawn(ControlledPawn, this);
		}
		else
		{
			UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: [EXIT BLOCKED] CanTouch=false but ControlledPawn {Pawn} does not implement PawnControlInterface – exit not performed.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Pawn", ControlledPawn->GetFName()));
		}
	}
}

/**
 * Handles input for ending/cancelling an interaction (e.g., releasing a "Use" button).
 *
 * Uses the interact controller interface to notify the controlled pawn/component to stop interacting.
 */
void AUnifiedPlayerController::StopInteracting()
{
	UActorComponent* InterfaceComponent = RegisteredComponentsByClass.FindRef(UInteractControlInterface::StaticClass());

	// If no registered component, try to stop interact on controlled pawn (e.g. after exit).
	APawn* ControlledPawn = GetPawn();
	if ((!InterfaceComponent || !InterfaceComponent->GetClass()->ImplementsInterface(UInteractControlInterface::StaticClass())) && ControlledPawn && ControlledPawn->GetClass()->ImplementsInterface(UPawnControlInterface::StaticClass()))
	{
		UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: [STOP] No registered component -> StopInteractOnPawn on {Pawn}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Pawn", ControlledPawn->GetFName()));
		IPawnControlInterface::Execute_StopInteractOnPawn(ControlledPawn, this);
		return;
	}

	if (!InterfaceComponent || !InterfaceComponent->GetClass()->ImplementsInterface(UInteractControlInterface::StaticClass()))
	{
		UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: [STOP] No valid InteractControllerInterface found.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}

	if (CanInteractWithComponent(InterfaceComponent))
	{
		UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: [STOP] CanTouch=true -> Execute_StopInteracting on {ComponentName}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("ComponentName", *InterfaceComponent->GetName()));
		IInteractControlInterface::Execute_StopInteracting(InterfaceComponent, this);
	}
	else
	{
		ControlledPawn = GetPawn();
		if (!ControlledPawn)
		{
			UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: [STOP] CanTouch=false but ControlledPawn is null.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		if (ControlledPawn->GetClass()->ImplementsInterface(UPawnControlInterface::StaticClass()))
		{
			UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: [STOP] CanTouch=false -> StopInteractOnPawn on {Pawn}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Pawn", ControlledPawn->GetFName()));
			IPawnControlInterface::Execute_StopInteractOnPawn(ControlledPawn, this);
		}
		else
		{
			UE_LOGFMT(Log_UnifiedController_Controls, Warning, "{Time}: {Line} {Class}: [STOP] Pawn {Pawn} does not implement PawnControlInterface.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Pawn", ControlledPawn->GetFName()));
		}
	}
}
#pragma endregion Interact