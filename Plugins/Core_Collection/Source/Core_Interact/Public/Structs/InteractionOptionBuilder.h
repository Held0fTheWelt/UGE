// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractionOption.h"

class IInteractableTarget;

/**
 * FInteractionOptionBuilder
 *
 *	Helper class for building interaction options.
 *	Adapted from FLyraInteractionOptionBuilder.
 */
struct CORE_INTERACT_API FInteractionOptionBuilder
{
public:
	FInteractionOptionBuilder(TScriptInterface<IInteractableTarget> InterfaceTargetScope, TArray<FInteractionOption>& InteractOptions);

	void AddInteractionOption(const FInteractionOption& Option);

private:
	TScriptInterface<IInteractableTarget> Scope;
	TArray<FInteractionOption>& Options;
};
