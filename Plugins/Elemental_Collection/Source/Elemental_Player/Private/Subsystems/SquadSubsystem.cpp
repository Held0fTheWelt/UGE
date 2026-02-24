// Copyright Epic Games, Inc. All Rights Reserved.

#include "Subsystems/SquadSubsystem.h"
#include "States/PlayerState_Elemental.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SquadSubsystem)

void USquadSubsystem::OnPlayerSquadIdChanged_Implementation(APlayerState_Elemental* PlayerState)
{
	// Base implementation: no-op. Override in Blueprint or subclass for squad UI/logic.
}
