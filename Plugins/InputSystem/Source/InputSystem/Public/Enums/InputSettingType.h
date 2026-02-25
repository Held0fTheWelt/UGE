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
 * File:        [InputSettingType.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "InputSettingType.generated.h"

/**
 * EInputSettingType
 *
 * Enumerates the various categories of input settings available in the project.
 * This enum allows you to distinguish between input configurations based on context or controlled entity type,
 * enabling modular and context-sensitive input management.
 */
UENUM(BlueprintType)
enum class EInputSettingType : uint8
{
    /** Default value. No input setting type specified. */
    IST_NONE         UMETA(DisplayName = "None"),

    /** Input settings specific to humanoid actors (e.g., players, NPCs, biped characters). */
    IST_HUMANOID     UMETA(DisplayName = "Humanoid"),

    /** Input settings tailored for vehicles (e.g., cars, trucks, ground-based vehicles). */
    IST_VEHICLE      UMETA(DisplayName = "Vehicle"),

    /** Input settings for turret-style entities (e.g., static or rotating gun turrets). */
    IST_TURRET       UMETA(DisplayName = "Turret"),

    /** Input settings for aircraft, including planes and fixed-wing vehicles. */
    IST_AIRCRAFT     UMETA(DisplayName = "Aircraft"),

    /** Input settings for helicopters or other rotorcraft. */
    IST_HELICOPTER   UMETA(DisplayName = "Helicopter"),

    /** Input settings specifically for weapons (could be used for modular weapon control schemes). */
    IST_WEAPON       UMETA(DisplayName = "Weapon"),

	/** Input settings for interaction, such as using objects or performing actions in the game world. */
    IST_INTERACT     UMETA(DisplayName = "Interact"),

	/** Input settings for controlling the engine of a vehicle or aircraft. */
    IST_CONTROL_ENGINE UMETA(DisplayName = "Engine Control"),
    
	/** Input settings for controlling the engine, such as throttle, ignition, or other engine-related actions. */
    IST_CONTROLS_ENGINE UMETA(DisplayName = "Engine Controls"),

	/** Input settings for controlling the camera, such as panning, zooming, or rotating. */
    IST_CONTROL_CAMERA UMETA(DisplayName = "Camera Control"),

    /** Input settings for controlling internal added systems of an entity, that manage working of that type of entity that can be controlled by input */
    IST_CONTROL_SYSTEMS UMETA(DisplayName = "Systems Control"),
};
