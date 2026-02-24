// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "UI/IndicatorSystem/CoreIndicatorLibrary.h"

#include "Components/CoreIndicatorManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreIndicatorLibrary)

class AController;

UCoreIndicatorLibrary::UCoreIndicatorLibrary()
{
}

UCoreIndicatorManagerComponent* UCoreIndicatorLibrary::GetIndicatorManagerComponent(AController* Controller)
{
	return UCoreIndicatorManagerComponent::GetComponent(Controller);
}
