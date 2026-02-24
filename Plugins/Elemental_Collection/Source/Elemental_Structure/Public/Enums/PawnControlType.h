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
 * File:        [PawnControlType.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "PawnControlType.generated.h"

UENUM(BlueprintType)
enum class EPawnControlType : uint8 {
    PCT_NONE            UMETA(DisplayName = "None"),
    PCT_HUMANOID        UMETA(DisplayName = "Humanoid"),
    PCT_VEHICLE         UMETA(DisplayName = "Vehicle"),
    PCT_AIRCRAFT        UMETA(DisplayName = "Aircraft"),
    PCT_HELICOPTER      UMETA(DisplayName = "Helicopter"),
    PCT_TURRET          UMETA(DisplayName = "Turret"),
    PCT_SHIP            UMETA(DisplayName = "Ship")
};