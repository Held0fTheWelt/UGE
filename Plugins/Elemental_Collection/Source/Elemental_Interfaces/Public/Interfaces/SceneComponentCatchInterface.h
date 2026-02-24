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
 * File:        SceneComponentCatchInterface.h
 * Created:     2025-06-12
 * Description: Interface for providing dynamic access to scene components by name.
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SceneComponentCatchInterface.generated.h"

 /**
  * USceneComponentCatchInterface
  *
  * Interface class for actors or objects that can return USceneComponent pointers by name.
  */
UINTERFACE(MinimalAPI, Blueprintable)
class USceneComponentCatchInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * ISceneComponentCatchInterface
 *
 * Implement this interface on any actor or UObject that needs to expose scene components
 * dynamically by name (for example: for attach points, gameplay targets, etc).
 */
class ELEMENTAL_INTERFACES_API ISceneComponentCatchInterface
{
	GENERATED_BODY()

public:
	/**
	 * Retrieve a scene component by name.
	 * @param ComponentName		Name identifier for the scene component (case sensitive).
	 * @return					Pointer to the found USceneComponent, or nullptr if not found.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces | SceneComponent")
	USceneComponent* GetSceneComponent(const FName& ComponentName) const;
};
