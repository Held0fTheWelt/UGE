// Copyright Epic Games, Inc. All Rights Reserved.

#include "Structs/InteractionOptionBuilder.h"
#include "Interfaces/IInteractableTarget.h"

FInteractionOptionBuilder::FInteractionOptionBuilder(TScriptInterface<IInteractableTarget> InterfaceTargetScope, TArray<FInteractionOption>& InteractOptions)
	: Scope(InterfaceTargetScope)
	, Options(InteractOptions)
{
}

void FInteractionOptionBuilder::AddInteractionOption(const FInteractionOption& Option)
{
	FInteractionOption& OptionEntry = Options.Add_GetRef(Option);
	OptionEntry.InteractableTarget = Scope;
}
