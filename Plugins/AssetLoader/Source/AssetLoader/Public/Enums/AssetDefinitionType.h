// AssetDefinitionType.h
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
 * File:        [AssetDefinitionType.h]
 * Created:     [2025-06-12]
 * Description: Enumeration of definition types for asset loading and DataTables
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionType.generated.h"

 /**
  * EAssetDefinitionType
  *
  * Defines the various categories of asset definitions that can be used
  * when loading DataTable entries or organizing asset registries.
  */
UENUM(BlueprintType)
enum class EAssetDefinitionType : uint8
{
    /** No asset definition; default placeholder */
    ADT_NONE            UMETA(DisplayName = "None"),

    /** Asset definitions that reference Actor classes */
    ADT_ACTOR           UMETA(DisplayName = "Actor Definition"),

    /** Asset definitions that reference Component classes */
    ADT_COMPONENT       UMETA(DisplayName = "Component Definition"),

	/** Asset definitions that reference input setups */
    ADT_INPUT           UMETA(DisplayName = "Input Definition"),

    /** Any other types of asset definitions not covered by actor/component */
    ADT_OTHER           UMETA(DisplayName = "Other Definition"),
};
