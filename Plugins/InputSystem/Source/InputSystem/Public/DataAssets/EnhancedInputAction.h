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
 * File:        [EnhancedInputAction.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Enums/InputActionType.h"
#include "Enums/InputSettingType.h"
#include "EnhancedInputAction.generated.h"

/**
 * UEnhancedInputAction
 *
 * Extension of UInputAction that adds metadata for context-sensitive input configuration.
 * Supports classification by pawn type and input action type for modular, scalable input systems.
 */
UCLASS()
class INPUTSYSTEM_API UEnhancedInputAction : public UInputAction
{
	GENERATED_BODY()

public:
	/**
	 * List of input setting types (e.g., Humanoid, Vehicle, Turret) this action is associated with.
	 * Allows selective enabling or filtering of input actions based on the controlled pawn or actor type.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Type)
	TArray<EInputSettingType> InputPawnTypes = TArray<EInputSettingType>();

	/**
	 * Logical type of this input action (e.g., Move, Jump, Interact, Shoot).
	 * Enables automated input mapping, UI display, or contextual decision-making based on action semantics.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Type)
	EInputActionType InputActionType = EInputActionType::IAT_NONE;
};
