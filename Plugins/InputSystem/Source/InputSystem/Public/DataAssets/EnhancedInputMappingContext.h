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
 * File:        [EnhancedInputMappingContext.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Enums/InputConfiguration.h"
#include "Enums/InputMappingType.h"
#include "Enums/InputSettingType.h"
#include "EnhancedInputMappingContext.generated.h"

/**
 * UEnhancedInputMappingContext
 *
 * Extension of UInputMappingContext that adds support for typed input mapping categories and pawn-type filtering.
 * Enables advanced filtering and management of input mapping contexts based on gameplay context or entity type.
 */
UCLASS()
class INPUTSYSTEM_API UEnhancedInputMappingContext : public UInputMappingContext
{
	GENERATED_BODY()

public:
	/**
	 * List of input setting types (e.g., Humanoid, Vehicle, Turret) this mapping context is intended for.
	 * Used to filter applicable input contexts for specific pawn or actor types at runtime.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Type)
	TArray<EInputSettingType> InputPawnTypes = TArray<EInputSettingType>();

	/**
	 * Defines the mapping context type (e.g., default, steering wheel).
	 * Enables more granular management and selection of input contexts based on hardware or gameplay scenario.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Type)
	EInputMappingType InputMappingType = EInputMappingType::IMT_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Configuration)
	EInputConfiguration InputConfiguration = EInputConfiguration::IC_NONE;
};
