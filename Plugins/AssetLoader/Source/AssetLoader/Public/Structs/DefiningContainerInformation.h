// InformativeAssetClassRef.h
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
 * File:        [InformativeAssetClassRef.h]
 * Created:     [2025-06-16]
 * Description: Base struct for referencing informative DataAssets by class
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Structs/ActorAndComponentSelector.h"
#include "DefiningContainerInformation.generated.h"

USTRUCT(BlueprintType)
struct FDefiningContainerInformation : public FTableRowBase
{
    GENERATED_BODY()

    FDefiningContainerInformation()
    {
		DefiningAssetName = NAME_None;
		DefinitionType = EAssetDefinitionType::ADT_NONE;
		TargetClasses.Empty();
    }

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
    EAssetDefinitionType DefinitionType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Registry")
    FName DefiningAssetName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Registry")
    TArray<FActorAndComponentSelector> TargetClasses;
};
