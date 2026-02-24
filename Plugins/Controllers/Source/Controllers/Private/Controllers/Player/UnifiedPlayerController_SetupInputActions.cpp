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
 * File:        [aaa.cpp]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#include "Controllers/Player/UnifiedPlayerController.h"

#include "DataAssets/EnhancedInputAction.h"
#include "DataAssets/EnhancedInputMappingContext.h"

#include "Enums/InputSettingType.h"
#include "Enums/InputActionType.h"
#include "Enums/InputMappingType.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Logging/Log_Controllers.h"

void AUnifiedPlayerController::SetupEnhancedInputAction(UEnhancedInputAction* InputAction)
{
	Super::SetupEnhancedInputAction(InputAction);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent))
	{
		UE_LOGFMT(Log_UnifiedController_Setup, Error, "{Time}: {Line} {Class}: EnhancedInputComponent is not valid",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}


	if (!IsValid(InputAction))
	{
		UE_LOGFMT(Log_UnifiedController_Setup, Error, "{Time}: {Line} {Class}: InputAction is not valid",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	// Lambda for easier logging
	auto LogBinding = [&](const FString& Func, ETriggerEvent Event) {
		UE_LOGFMT(Log_UnifiedController_Setup, VeryVerbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to {Func} with event {Event}",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()), ("Func", Func), ("Event", *StaticEnum<ETriggerEvent>()->GetNameStringByValue((int64)Event)));
		};

	switch (InputAction->InputActionType)
	{
		// Actions for all pawns
	case EInputActionType::IAT_TOGGLECAMERA:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::ToggleCamera);
		LogBinding(TEXT("ToggleCamera"), ETriggerEvent::Started);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to ToggleCamera with event Started",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_LOOK:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AUnifiedPlayerController::Look);
		LogBinding(TEXT("Look"), ETriggerEvent::Triggered);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Look with event Triggered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_ZOOMCAMERA:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AUnifiedPlayerController::ZoomCamera);
		LogBinding(TEXT("Zoom"), ETriggerEvent::Triggered);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to ZoomCamera with event Triggered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_INTERACT:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::Interact);
		LogBinding(TEXT("Interact"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::StopInteracting);
		LogBinding(TEXT("StopInteracting"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Interact with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
		// Humanoid
	case EInputActionType::IAT_MOVE:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AUnifiedPlayerController::Move);
		LogBinding(TEXT("Move"), ETriggerEvent::Triggered);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Move with event Triggered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_JUMP:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::Jump);
		LogBinding(TEXT("Jump"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::StopJumping);
		LogBinding(TEXT("StopJumping"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Jump with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_RUN:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::Run);
		LogBinding(TEXT("Run"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::StopRunning);
		LogBinding(TEXT("StopRunning"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Run with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;

		// Vehicle
	case EInputActionType::IAT_THROTTLE:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AUnifiedPlayerController::Throttle);
		LogBinding(TEXT("Throttle"), ETriggerEvent::Triggered);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::Throttle);
		LogBinding(TEXT("Throttle"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Throttle with event Triggered and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_STEER:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AUnifiedPlayerController::Steering);
		LogBinding(TEXT("Steering"), ETriggerEvent::Triggered);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::SteeringCompleted);
		LogBinding(TEXT("SteeringCompleted"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Steering with events Triggered and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_BRAKE:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AUnifiedPlayerController::Brake);
		LogBinding(TEXT("Brake"), ETriggerEvent::Triggered);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::StartBrake);
		LogBinding(TEXT("StartBrake"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::StopBrake);
		LogBinding(TEXT("StopBrake"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Brake with events Triggered, Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_HANDBRAKE:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::StartHandbrake);
		LogBinding(TEXT("StartHandbrake"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::StopHandbrake);
		LogBinding(TEXT("StopHandbrake"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Handbrake with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_RESET:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AUnifiedPlayerController::ResetVehicle);
		LogBinding(TEXT("ResetVehicle"), ETriggerEvent::Triggered);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to ResetVehicle with event Triggered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;

		// Turret
	case EInputActionType::IAT_ROTATE:
		//EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AUnifiedPlayerController::Rotate);
		//LogBinding(TEXT("Rotate"), ETriggerEvent::Triggered);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AUnifiedPlayerController::Look);
		LogBinding(TEXT("Look"), ETriggerEvent::Triggered);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Rotate with event Triggered",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
		// Helicopter
	case EInputActionType::IAT_CYCLIC:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::Cyclic);
		LogBinding(TEXT("Cyclic"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::CyclicCompleted);
		LogBinding(TEXT("CyclicCompleted"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Cyclic with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_CYCLIC_KEY_W:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::CyclicKeyWStart);
		LogBinding(TEXT("CyclicKeyWStart"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::CyclicKeyWStop);
		LogBinding(TEXT("CyclicKeyWStop"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to CyclicKeyW with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_CYCLIC_KEY_S:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::CyclicKeySStart);
		LogBinding(TEXT("CyclicKeySStart"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::CyclicKeySStop);
		LogBinding(TEXT("CyclicKeySStop"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to CyclicKeyS with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_CYCLIC_KEY_A:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::CyclicKeyAStart);
		LogBinding(TEXT("CyclicKeyAStart"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::CyclicKeyAStop);
		LogBinding(TEXT("CyclicKeyAStop"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to CyclicKeyA with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_CYCLIC_KEY_D:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::CyclicKeyDStart);
		LogBinding(TEXT("CyclicKeyDStart"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::CyclicKeyDStop);
		LogBinding(TEXT("CyclicKeyDStop"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to CyclicKeyD with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_COLLECTIVE:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AUnifiedPlayerController::Collective);
		LogBinding(TEXT("Collective"), ETriggerEvent::Triggered);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::Collective);
		LogBinding(TEXT("Collective"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Collective with event Triggered and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_COLLECTIVE_KEY:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::CollectiveKeyStart);
		LogBinding(TEXT("CollectiveKeyStart"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::CollectiveKeyStop);
		LogBinding(TEXT("CollectiveKeyStop"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to CollectiveKey with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_PEDALS:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AUnifiedPlayerController::Pedals);
		LogBinding(TEXT("Pedals"), ETriggerEvent::Triggered);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::Pedals);
		LogBinding(TEXT("Pedals"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to Pedals with event Triggered and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_PEDALS_KEY_LEFT:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::PedalsKeyLeftStart);
		LogBinding(TEXT("PedalsKeyLeftStart"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::PedalsKeyLeftStop);
		LogBinding(TEXT("PedalsKeyLeftStop"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to PedalsKeyLeft with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));			
		break;
	case EInputActionType::IAT_PEDALS_KEY_RIGHT:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::PedalsKeyRightStart);
		LogBinding(TEXT("PedalsKeyRightStart"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::PedalsKeyRightStop);
		LogBinding(TEXT("PedalsKeyRightStop"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to PedalsKeyRight with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
		// Engine
	case EInputActionType::IAT_ENGINE_SWITCH:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::StartEngineAction);
		LogBinding(TEXT("StartEngineAction"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::StopEngineAction);
		LogBinding(TEXT("StopEngineAction"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to StartEngineAction and StopEngineAction with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_ENGINE_2ND_SWITCH:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::StartSecondEngineAction);
		LogBinding(TEXT("Start2ndEngineAction"), ETriggerEvent::Started);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AUnifiedPlayerController::StopSecondEngineAction);
		LogBinding(TEXT("Stop2ndEngineAction"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to StartSecondEngineAction and StopSecondEngineAction with events Started and Completed",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	case EInputActionType::IAT_SAS_TOGGLE:
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AUnifiedPlayerController::ToggleSASAction);
		LogBinding(TEXT("ToggleSASAction"), ETriggerEvent::Started);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} bound to ToggleSASAction with event Started",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
		// Weapon/Other
	case EInputActionType::IAT_SHOOT:
		// Bind your shoot logic here
		//LogBinding(TEXT("Shoot (Not implemented)"), ETriggerEvent::Completed);
		UE_LOGFMT(Log_UnifiedController_Setup, Verbose, "{Time}: {Line} {Class}: InputAction {ActionName} is not implemented for Shoot action",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME),
			("ActionName", *InputAction->GetName()));
		break;
	default:
		break;
	}
}