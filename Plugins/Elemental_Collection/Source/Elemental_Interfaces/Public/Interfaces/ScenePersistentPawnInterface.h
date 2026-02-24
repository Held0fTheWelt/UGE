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
 * File:        [ScenePersistentPawnInterface.h]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ScenePersistentPawnInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UScenePersistentPawnInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API IScenePersistentPawnInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	bool ShouldDespawnOnUnpossess() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	int32 AddReferencedPawnToLoad(class AActor* ActorToAdd);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	class AActor* GetReferencedPawnFromLoad(const int32& Index);
};
