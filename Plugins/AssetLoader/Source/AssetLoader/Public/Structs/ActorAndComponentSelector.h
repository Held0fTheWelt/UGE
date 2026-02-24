// ActorAndComponentDefinitionRow.h
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
 * File:        [ActorAndComponentDefinitionRow.h]
 * Created:     [2025-06-16]
 * Description: Common base struct for entity definitions (name + asset class)
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "Enums/AssetDefinitionType.h"
#include "Structs/InformativeAssetClassRef.h"
#include "ActorAndComponentSelector.generated.h"

 /**
  * Shared base for actor and component definition rows.
  */
USTRUCT(BlueprintType)
struct FActorAndComponentSelector
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Registry")
    TSoftClassPtr<class UObject> TargetClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Definition")
    TArray<FInformativeAssetClassRef> AssetDefinitions;
};
