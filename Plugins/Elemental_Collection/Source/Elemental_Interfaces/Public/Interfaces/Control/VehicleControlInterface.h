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
 * File:        VehiclesInterface.h
 * Created:     2025-06-12
 * Description: Interface for vehicle input actions (steering, throttle, brake, etc.).
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VehicleControlInterface.generated.h"

 /**
  * UVehiclesInterface
  * Unreal interface class for vehicle-related input actions.
  * Marked Blueprintable to support Blueprint classes as well.
  */
UINTERFACE(MinimalAPI, Blueprintable)
class UVehicleControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IVehiclesInterface
 * Interface for any vehicle pawn or component that handles driving-related actions.
 * Implement this interface on your vehicle pawn/actor to connect input events to simulation logic.
 */
class ELEMENTAL_INTERFACES_API IVehicleControlInterface
{
	GENERATED_BODY()

public:

	/** Called for steering input (e.g. left/right wheel or stick, -1..+1) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle Actions")
	void Steering(float Value);

	/** Called when steering input ends or is reset */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle Actions")
	void StopSteering();

	/** Called for brake input (e.g. brake pedal, 0..+1) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle Actions")
	void Brake(float Value);

	/** Called when the brake is actively started (e.g. input pressed) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle Actions")
	void StartBrake();

	/** Called when the brake is released (e.g. input released) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle Actions")
	void StopBrake();

	/** Called when the handbrake is actively started (e.g. input pressed) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle Actions")
	void StartHandbrake();

	/** Called when the handbrake is released (e.g. input released) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle Actions")
	void StopHandbrake();

	/** Called for camera look-around input (mouse/gamepad/trackir, typically -1..+1 or degree delta) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle Actions")
	void Look(float Value);

	/** Called to reset the vehicle (e.g. for recovery after flipping over) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vehicle Actions")
	void ResetVehicle();
};
