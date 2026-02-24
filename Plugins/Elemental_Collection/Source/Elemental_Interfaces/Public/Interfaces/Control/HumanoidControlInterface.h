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
 * File:        HumanoidsInterface.h
 * Created:     2025-06-12
 * Description: Interface for humanoid pawn/player actions, such as movement,
 *              camera, and basic interactions. Intended for use with Enhanced Input
 *              and generic gameplay controller systems.
 * -------------------------------------------------------------------------------
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HumanoidControlInterface.generated.h"

 /**
  * UHumanoidsInterface
  * Unreal Interface wrapper for IHumanoidsInterface.
  * Do not implement any logic here.
  */
UINTERFACE(MinimalAPI, Blueprintable)
class UHumanoidControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IHumanoidsInterface
 *
 * Interface for actors (usually pawns/characters) to implement core humanoid
 * input-driven actions. BlueprintNativeEvent for Blueprint or C++ override support.
 */
class ELEMENTAL_INTERFACES_API IHumanoidControlInterface
{
	GENERATED_BODY()

public:

	/** Called for jump input (press) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Actions")
	void Jump();

	/** Called for jump input (release) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Actions")
	void StopJumping();

	/** Called for run/sprint input (press) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Actions")
	void Run();

	/** Called for run/sprint input (release) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Actions")
	void StopRunning();

	/** Called for 2D movement input (X: Forward/Backward, Y: Right/Left) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Actions")
	void Move(const FVector2D& Value);

	/** Called for 2D look input (X: Yaw, Y: Pitch) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Actions")
	void Look(const FVector2D& Value);

#if WITH_EDITOR
	/** Called to update the humanoid pawn's input settings in the editor */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Actions Owner")
	FName GetFriendlyName() const;
#endif // WITH_EDITOR
};
