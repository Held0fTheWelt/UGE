// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Equipment/EquipmentDefinition.h"
#include "Equipment/EquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(EquipmentDefinition)

UEquipmentDefinition::UEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UEquipmentInstance::StaticClass();
}
