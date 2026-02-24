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
 * File:        [ViewTypeConfiguration.h]
 * Created:     [2025-06-12]
 * Description: Defines all configuration parameters for camera view types
 *              (e.g. first person, third person, top view, etc.).
 *              Used to control SpringArm and camera behavior per view mode.
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums/CameraViewType.h"
#include "ViewTypeConfiguration.generated.h"

 /**
  * FViewTypeConfiguration
  *
  * A data structure for defining per-view-type camera/springarm configuration.
  * Used for data-driven camera behavior, e.g. on vehicles or characters.
  * Each entry specifies how the spring arm and camera should be set up for a particular view type.
  */
USTRUCT(BlueprintType)
struct ELEMENTAL_STRUCTURE_API FViewTypeConfiguration : public FTableRowBase
{
	GENERATED_BODY()

public:
	FViewTypeConfiguration()
	{
		ViewType = ECameraViewType::CVT_NONE;
		SceneComponentToAttachName = TEXT("Mesh");
		SpringArmTransform = FTransform();
		SpringArmLength = 300.0f;
		SocketOffset = FVector(0.0f, 0.0f, 50.0f);
		TargetOffset = FVector(0.0f, 0.0f, 50.0f);
		bCameraRotationLag = true;
		CameraRotationLagSpeed = 5.0f;
		bInheritPitch = false;
		bInheritYaw = true;
		bInheritRoll = false;
		bDoCollisionTest = false;
	}

	/** Camera view type this configuration applies to (e.g. first, third, top). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera View Type")
	ECameraViewType ViewType;

	/** Name of the scene component to which the spring arm/camera should attach (default: Mesh). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Springarm")
	FName SceneComponentToAttachName;

	/** Local location offset for the spring arm relative to its parent (e.g. behind/above pawn). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Springarm")
	FTransform SpringArmTransform;

	/** Arm length from the pivot to the camera (how far back the camera sits). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Springarm")
	float SpringArmLength;

	/** Socket offset for camera positioning (e.g. move the camera up/down/sideways from the arm tip). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Springarm")
	FVector SocketOffset;

	/** Target offset for camera focus/center (e.g. where the camera looks at on the pawn). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Springarm")
	FVector TargetOffset;

	/** Whether to enable camera rotation lag (smooth camera rotation transitions). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Springarm")
	bool bCameraRotationLag;

	/** Speed at which the camera rotation lags behind input (if enabled). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Springarm")
	float CameraRotationLagSpeed;

	/** Whether spring arm inherits pawn pitch (vertical rotation). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Springarm")
	bool bInheritPitch;

	/** Whether spring arm inherits pawn yaw (horizontal rotation). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Springarm")
	bool bInheritYaw;

	/** Whether spring arm inherits pawn roll (side tilt). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Springarm")
	bool bInheritRoll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bInvertPitch = false;

	/** Enable collision checks for the spring arm (camera avoids clipping through world). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Springarm")
	bool bDoCollisionTest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bAutoActivate = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bUsePawnControlRotation = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bUsePawnControlRotationOnCamera = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Orchestrator")
	bool bCanZoom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Orchestrator")
	float ZoomMin = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Orchestrator")
	float ZoomMax = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Orchestrator")
	float ZoomChangeModifier = 1.0f;
};
