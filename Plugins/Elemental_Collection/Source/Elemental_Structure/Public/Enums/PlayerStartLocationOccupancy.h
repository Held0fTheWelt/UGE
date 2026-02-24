#pragma once

#include "CoreMinimal.h"
#include "PlayerStartLocationOccupancy.generated.h"

UENUM()
enum class EPlayerStartLocationOccupancy : uint8
{
	Empty,
	Partial,
	Full
};