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
 * File:        [SpawnInstanceSubsystem.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SpawnInstanceSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_SpawnInstanceSubsystem, Log, All);

/**
 * 
 */
UCLASS()
class ELEMENTAL_CLASSES_API USpawnInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	class AActor* SpawnDefaultActor(TSubclassOf<class AActor> ActorToSpawn, const FTransform& InTransform);
	class AActor* SpawnDefaultActorDeferred(TSubclassOf<class AActor> ActorToSpawn, const FTransform& InTransform);
	void SpawnDefaultPawn(class APlayerController* PlayerController, const FTransform& InTransform);
};
