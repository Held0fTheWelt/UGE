// Copyright Epic Games, Inc. All Rights Reserved.

#include "Subsystems/AttributeValueByTagSubsystemRegistry.h"
#include "Subsystems/AttributeValueByTagSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AttributeValueByTagSubsystemRegistry)

void UAttributeValueByTagSubsystemRegistry::SetSubsystem(UAttributeValueByTagSubsystem* InSubsystem)
{
	Subsystem = InSubsystem;
}
