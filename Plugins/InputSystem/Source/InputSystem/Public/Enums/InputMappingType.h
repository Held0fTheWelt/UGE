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
 * File:        [InputMappingType.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "InputMappingType.generated.h"

/**
 * EInputMappingType
 *
 * Enumerates supported categories of input mapping contexts within the project.
 * This enum enables the input system to select or filter mapping contexts based on hardware or gameplay requirements,
 * supporting modular input profiles for various devices and use cases.
 */
UENUM(BlueprintType)
enum class EInputMappingType : uint8
{
    /** Default value. Represents no mapping context. */
    IMT_NONE             UMETA(DisplayName = "None"),

    /** Standard input mapping context, typically used for keyboard, mouse, or gamepad. */
    IMT_DEFAULT          UMETA(DisplayName = "Default Mapping Context"),

    /** Input mapping context optimized for steering wheel peripherals or similar devices. */
    IMT_STEERING_WHEEL   UMETA(DisplayName = "Steering Wheel"),

    /** Placeholder value reserved for future expansion or undefined mapping types. */
    IMT_                 UMETA(DisplayName = "Placeholder"),
};
