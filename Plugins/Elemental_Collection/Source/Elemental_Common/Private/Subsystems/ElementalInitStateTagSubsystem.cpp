// Copyright Epic Games, Inc. All Rights Reserved.

#include "Subsystems/ElementalInitStateTagSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ElementalInitStateTagSubsystem)

void UElementalInitStateTagSubsystem::SetInitStateTags(FGameplayTag InSpawned, FGameplayTag InDataAvailable, FGameplayTag InDataInitialized, FGameplayTag InGameplayReady)
{
	InitState_Spawned = InSpawned;
	InitState_DataAvailable = InDataAvailable;
	InitState_DataInitialized = InDataInitialized;
	InitState_GameplayReady = InGameplayReady;
}
