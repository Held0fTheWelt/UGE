#pragma once

#include "CoreMinimal.h"
#include "PlayerConnectionType.generated.h"

/** Defines the types of client connected */
UENUM()
enum class EPlayerConnectionType : uint8
{
	// An active player
	Player = 0,

	// Spectator connected to a running game
	LiveSpectator,

	// Spectating a demo recording offline
	ReplaySpectator,

	// A deactivated player (disconnected)
	InactivePlayer
};