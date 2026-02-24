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
 * File:        PhysicalSenseInterface.h
 * Created:     2025-06-12
 * Description: Interface for actors/components to participate in physical sense systems (touch, etc.)
 * -------------------------------------------------------------------------------
 */
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/SenseType.h"
#include "PhysicalSenseInterface.generated.h"

 /**
  * UPhysicalSenseInterface
  * Unreal Engine base interface class for sense features.
  */
UINTERFACE(MinimalAPI, Blueprintable)
class UPhysicalSenseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IPhysicalSenseInterface
 *
 * Interface for any actor or component that should participate in a humanoid's sense system
 * (e.g. for registering, forgetting, and querying touchable/nearby objects).
 */
class ELEMENTAL_INTERFACES_API IPhysicalSenseInterface
{
	GENERATED_BODY()

public:
	/** Returns true if the given actor can currently be touched */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Sense")
	bool CanTouchActor(const AActor* Actor) const;

	/** Returns true if the currently "focused" actor can be touched */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Sense")
	bool CanTouchCurrentActor() const;

	/** Returns true if the character is currently touching the focused actor */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Sense")
	bool IsTouchingCurrentActor() const;

	/** Set the current touch target (may be null) */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Sense")
	void SetTouchingCurrentActor(const AActor* Actor);

	/** Get the current touch target, if any */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Sense")
	AActor* GetCurrentTouchActor() const;

	/** Get all currently touchable actors */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Humanoid Sense")
	TArray<TSoftObjectPtr<AActor>> GetTouchableActors() const;
};
