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
 * File:        CameraControlInterface.h
 * Created:     2025-06-12
 * Description: Interface for any actor or component wishing to handle camera look input.
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraControlInterface.generated.h"

 /**
  * UCameraControlInterface
  *
  * Unreal Engine interface base class for blueprints and C++.
  * You should NOT implement methods here.
  */
UINTERFACE(MinimalAPI, Blueprintable)
class UCameraControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * ICameraControlInterface
 *
 * Interface for any class (actor or component) that wants to receive/look input.
 * Implement this in your pawn, controller, or any component to handle camera look input.
 */
class ELEMENTAL_INTERFACES_API ICameraControlInterface
{
	GENERATED_BODY()

public:
	/**
	 * Called to apply look/mouse/stick input to the camera.
	 * @param Input	The input axis vector (typically X = yaw, Y = pitch).
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera Control")
	void Look(const FVector2D& Input);

	/** Called for camera toggle (first/third person etc.)
	@param bForward  If true, cycle camera forward; if false, backward. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera Control")
	void ToggleCamera(bool bForward);

	/** Called to apply zoom input to the camera.
	 * @param Value The zoom value, typically a float where positive values zoom in and negative values zoom out. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera Control")
	void ZoomCamera(float Value);

#if WITH_EDITOR
	/** Called to update the humanoid pawn's input settings in the editor */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Actions Owner")
	FName GetFriendlyName() const;
#endif // WITH_EDITOR
};
