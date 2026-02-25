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
 * File:        [InputConfiguration.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "InputConfiguration.generated.h"

UENUM(BlueprintType)
enum class EInputConfiguration : uint8
{
    /** Default value. Represents no action. */
    IC_NONE                 UMETA(DisplayName = "None"),
    IC_CONFIGURATION_1      UMETA(DisplayName = "Configuration 1"),
	IC_CONFIGURATION_2      UMETA(DisplayName = "Configuration 2"),
	IC_CONFIGURATION_3      UMETA(DisplayName = "Configuration 3"),
	IC_CONFIGURATION_4      UMETA(DisplayName = "Configuration 4"),
	IC_CONFIGURATION_5      UMETA(DisplayName = "Configuration 5"),
	IC_CONFIGURATION_6      UMETA(DisplayName = "Configuration 6"),
	IC_CONFIGURATION_7      UMETA(DisplayName = "Configuration 7"),
	IC_CONFIGURATION_8      UMETA(DisplayName = "Configuration 8"),
	IC_CONFIGURATION_9      UMETA(DisplayName = "Configuration 9"),
	IC_CONFIGURATION_10     UMETA(DisplayName = "Configuration 10"),	
};