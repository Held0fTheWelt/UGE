// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractionInstigator.generated.h"

class AActor;
class UObject;
struct FFrame;

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UInteractionInstigator : public UInterface
{
	GENERATED_BODY()
};

/**
 * IInteractionInstigator
 *
 *	Interface for actors that can instigate interactions.
 *	Adapted from ILyraInteractionInstigator.
 */
class GAMEFEATURE_INTERACTIONSYSTEMRUNTIME_API IInteractionInstigator
{
	GENERATED_BODY()

public:
	/**
	 * Returns the actor that should be used as the instigator for interactions.
	 */
	virtual AActor* GetInteractionInstigatorActor() const { return nullptr; }
};
