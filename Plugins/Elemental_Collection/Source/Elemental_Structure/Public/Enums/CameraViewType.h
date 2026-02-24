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
 * File:        [CameraViewType.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "CameraViewType.generated.h"

UENUM(BlueprintType)
enum class ECameraViewType : uint8 {
    CVT_NONE            UMETA(DisplayName = "None"),
    CVT_FIRST           UMETA(DisplayName = "First Person"),
    CVT_THIRD           UMETA(DisplayName = "Third Person"),
    CVT_TOP             UMETA(DisplayName = "Top View"),
    CVT_ABOVE           UMETA(DisplayName = "Above View"),
    CVT_FRONT_TO_REAR   UMETA(DisplayName = "Front to Rear"),
    CVT_PANORAMIC       UMETA(DisplayName = "Panoramic View"),
    CVT_COPILOT         UMETA(DisplayName = "CoPilot View"),
    CVT_DOOR_LEFT       UMETA(DisplayName = "Left Door View"),
    CVT_DOOR_RIGHT      UMETA(DisplayName = "Right Door View"),
    CVT_FRONT_ACTION    UMETA(DisplayName = "Front Action View"),
    CVT_BACK            UMETA(DisplayName = "Back View"),
    CVT_TAIL            UMETA(DisplayName = "Tail View"),
    CVT_COCKPIT         UMETA(DisplayName = "Cockpit View"),
};