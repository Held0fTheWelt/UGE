// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Components/CoreIndicatorManagerComponent.h"

#include "UI/IndicatorSystem/CoreIndicatorDescriptor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreIndicatorManagerComponent)

UCoreIndicatorManagerComponent::UCoreIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoRegister = true;
	bAutoActivate = true;
}

/*static*/ UCoreIndicatorManagerComponent* UCoreIndicatorManagerComponent::GetComponent(AController* Controller)
{
	if (Controller)
	{
		return Controller->FindComponentByClass<UCoreIndicatorManagerComponent>();
	}

	return nullptr;
}

void UCoreIndicatorManagerComponent::AddIndicator(UCoreIndicatorDescriptor* IndicatorDescriptor)
{
	IndicatorDescriptor->SetIndicatorManagerComponent(this);
	OnIndicatorAdded.Broadcast(IndicatorDescriptor);
	Indicators.Add(IndicatorDescriptor);
}

void UCoreIndicatorManagerComponent::RemoveIndicator(UCoreIndicatorDescriptor* IndicatorDescriptor)
{
	if (IndicatorDescriptor)
	{
		ensure(IndicatorDescriptor->GetIndicatorManagerComponent() == this);
	
		OnIndicatorRemoved.Broadcast(IndicatorDescriptor);
		Indicators.Remove(IndicatorDescriptor);
	}
}
