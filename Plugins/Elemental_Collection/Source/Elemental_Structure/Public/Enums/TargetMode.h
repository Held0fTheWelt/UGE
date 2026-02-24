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
 * File:        [ViewMode.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "TargetMode.generated.h"

UENUM(BlueprintType)
enum class ETargetMode : uint8 {
    TM_NONE             UMETA(DisplayName = "None"),
    TM_DIRECT           UMETA(DisplayName = "Direct Targeting"),
    TM_BALLISTIC_HIGH   UMETA(DisplayName = "Ballistic High Arc Targeting"),
    TM_BALLISTIC_LOW    UMETA(DisplayName = "Ballistic Low Arc Targeting"),
};