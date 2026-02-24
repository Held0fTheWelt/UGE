// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "UI/Frontend/CoreFrontendStateComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreFrontendStateComponent)

UCoreFrontendStateComponent::UCoreFrontendStateComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoRegister = true;
	bAutoActivate = true;
}

/*static*/ UCoreFrontendStateComponent* UCoreFrontendStateComponent::GetComponent(AController* Controller)
{
	if (Controller)
	{
		return Controller->FindComponentByClass<UCoreFrontendStateComponent>();
	}

	return nullptr;
}

void UCoreFrontendStateComponent::SetIsInFrontend(bool bInFrontend)
{
	if (bIsInFrontend != bInFrontend)
	{
		bIsInFrontend = bInFrontend;
		OnFrontendStateChanged.Broadcast(bIsInFrontend);
	}
}
