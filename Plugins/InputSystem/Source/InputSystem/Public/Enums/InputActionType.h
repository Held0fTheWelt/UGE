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
 * File:        [InputActionType.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "InputActionType.generated.h"

/**
 * EInputActionType
 *
 * Enumerates all supported types of input actions within the project.
 * This enum provides a strongly-typed mechanism to distinguish between various input behaviors,
 * enabling more readable, maintainable, and scalable input configuration and processing.
 */
UENUM(BlueprintType)
enum class EInputActionType : uint8
{
    /** Default value. Represents no action. */
    IAT_NONE            UMETA(DisplayName = "None"),

    /** Camera look input (e.g., mouse or gamepad stick movement for camera orientation). */
    IAT_LOOK                UMETA(DisplayName = "Look"),

    /** Toggle between available camera views or modes. */
    IAT_TOGGLECAMERA        UMETA(DisplayName = "Toggle Camera"),

	/** Zoom camera input, typically used for zooming in or out on the camera view. */
    IAT_ZOOMCAMERA        UMETA(DisplayName = "Zoom Camera"),

    /** General-purpose interact input (e.g., picking up objects, activating UI, opening doors). */
    IAT_INTERACT            UMETA(DisplayName = "Interact"),

    /** Jump action, typically mapped to character jump behavior. */
    IAT_JUMP                UMETA(DisplayName = "Jump"),

    /** Move action, for character or vehicle directional movement. */
    IAT_MOVE                UMETA(DisplayName = "Move"),

    /** Run or sprint action, toggling increased movement speed. */
    IAT_RUN                 UMETA(DisplayName = "Run"),

    /** Throttle control, used for vehicles or analog speed adjustments. */
    IAT_THROTTLE            UMETA(DisplayName = "Throttle"),
    /** Throttle control, used for vehicles or analog speed adjustments. */
    IAT_THROTTLE_KEYUP      UMETA(DisplayName = "Throttle Key Up"),
    /** Throttle control, used for vehicles or analog speed adjustments. */
    IAT_THROTTLE_KEYDOWN    UMETA(DisplayName = "Throttle Key Down"),

    /** Steering input, used for directional control in vehicles. */
    IAT_STEER               UMETA(DisplayName = "Steer"),

    /** Brake action, typically mapped to slowing or stopping vehicles. */
    IAT_BRAKE               UMETA(DisplayName = "Brake"),

    /** Handbrake action for vehicles, often used for sharp turns or skidding. */
    IAT_HANDBRAKE           UMETA(DisplayName = "Handbrake"),

    /** Reset action, commonly used to reset player or vehicle state. */
    IAT_RESET               UMETA(DisplayName = "Reset"),

    /** Shoot or primary fire action, mapped to weapons or tools. */
    IAT_SHOOT               UMETA(DisplayName = "Shoot"),

    /** Rotate a rotating component. */
    IAT_ROTATE              UMETA(DisplayName = "Rotate"),

	/** Collective input for vehicles, typically used in flight or advanced vehicle controls. */
    IAT_COLLECTIVE          UMETA(DisplayName = "Collective"),

	/** Collective keys input for vehicles, typically used in flight or advanced vehicle controls. */
    IAT_COLLECTIVE_KEY      UMETA(DisplayName = "Collective Key"),

	/** Cyclic input for vehicles, typically used in flight controls. */
    IAT_CYCLIC              UMETA(DisplayName = "Cyclic"),

	/** Cyclic keys input for vehicles, typically used for directional control in flight. */
    IAT_CYCLIC_KEY_W        UMETA(DisplayName = "Cyclic Key W"),

    /** Cyclic keys input for vehicles, typically used for directional control in flight. */
    IAT_CYCLIC_KEY_A        UMETA(DisplayName = "Cyclic Key A"),

	/** Cyclic keys input for vehicles, typically used for directional control in flight. */
    IAT_CYCLIC_KEY_S        UMETA(DisplayName = "Cyclic Key S"),

	/** Cyclic keys input for vehicles, typically used for directional control in flight. */
    IAT_CYCLIC_KEY_D        UMETA(DisplayName = "Cyclic Key D"),

	/** Pedals input for vehicles, typically used for flight or advanced vehicle controls. */
    IAT_PEDALS              UMETA(DisplayName = "Pedals"),

	/** Pedals keys input for vehicles, typically used for flight or advanced vehicle controls. */
    IAT_PEDALS_KEY_LEFT     UMETA(DisplayName = "Pedals Key Left"),

	/** Pedals keys input for vehicles, typically used for flight or advanced vehicle controls. */
    IAT_PEDALS_KEY_RIGHT    UMETA(DisplayName = "Pedals Key Right"),

	/** Start and stop engine action, used for vehicles or machinery. */ 
    IAT_ENGINE_SWITCH       UMETA(DisplayName = "Start and Stop Engine"),

    /** Second engine switch action, for additional control. */
    IAT_ENGINE_2ND_SWITCH   UMETA(DisplayName = "Start and Stop Second Engine"),

    /** Toggle Stability Augmented System (SAS) on/off. */
    IAT_SAS_TOGGLE          UMETA(DisplayName = "SAS Toggle"),

    /** Placeholder value, reserved for future expansion or undefined actions. */
    IAT_PLACEHOLDER     UMETA(DisplayName = "Placeholder"),
};
