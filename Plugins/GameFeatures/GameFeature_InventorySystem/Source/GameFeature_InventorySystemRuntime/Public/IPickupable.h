// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPickupable.generated.h"

class AActor;
class UObject;
struct FFrame;

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UPickupable : public UInterface
{
	GENERATED_BODY()
};

/**
 * IPickupable
 *
 *	Interface for actors that can be picked up.
 *	Adapted from ILyraPickupable.
 */
class GAMEFEATURE_INVENTORYSYSTEMRUNTIME_API IPickupable
{
	GENERATED_BODY()

public:
	/**
	 * Called when this item is picked up.
	 * @param Instigator The actor that picked up this item
	 */
	virtual void OnPickedUp(AActor* Instigator) {}
};
