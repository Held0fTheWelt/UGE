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
 * File:        InteractControllerInterface.h
 * Created:     2025-06-12
 * Description: Interface for pawn/controller interaction logic (start/stop).
 * -------------------------------------------------------------------------------
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractControlInterface.generated.h"

 /**
  * UInteractControllerInterface
  * Unreal Engine interface object for controller-driven interaction.
  * BlueprintType for use in Blueprint classes.
  */
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IInteractControllerInterface
 *
 * Interface for controllers or pawns that can interact and stop interacting with objects.
 * Intended for input-driven interaction systems.
 */
class ELEMENTAL_INTERFACES_API IInteractControlInterface
{
	GENERATED_BODY()

public:
	/** Called to initiate controller-driven interaction (e.g., "Use" or "Interact" input) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Actions")
	void Interact(class AController* InstigatorController);

	/** Called to stop controller-driven interaction (e.g., on release or interruption) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Actions")
	void StopInteracting(class AController* InstigatorController);
};
