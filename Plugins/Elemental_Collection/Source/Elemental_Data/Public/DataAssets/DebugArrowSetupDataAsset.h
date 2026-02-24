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
 * File:        [DebugArrowSetupDataAsset.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DebugArrowSetupDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ELEMENTAL_DATA_API UDebugArrowSetupDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UDebugArrowSetupDataAsset()
	{
		// Default values can be set here if needed
		RelativeTransform = FTransform::Identity;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Arrow")
	FTransform RelativeTransform;
};
