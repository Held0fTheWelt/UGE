// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractableTarget.generated.h"

class AActor;
class AController;
class UObject;
struct FFrame;
struct FInteractionQuery;
struct FInteractionOptionBuilder;
struct FGameplayEventData;
struct FGameplayTag;

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UInteractableTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * IInteractableTarget
 *
 *	Interface for actors that can be interacted with.
 *	Adapted from ILyraInteractableTarget.
 */
class CORE_INTERFACES_API IInteractableTarget
{
	GENERATED_BODY()

public:
	/**
	 * Gets the interaction options available for the specified instigator.
	 * @param InteractQuery The interaction query parameters
	 * @param OptionBuilder Builder for adding interaction options
	 */
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) {}

	/**
	 * Customize the interaction event data before it's sent.
	 */
	virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutEventData) {}
};
