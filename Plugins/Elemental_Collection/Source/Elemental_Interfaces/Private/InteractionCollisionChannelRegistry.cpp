// Copyright Epic Games, Inc. All Rights Reserved.

#include "Interfaces/InteractionCollisionChannelProvider.h"

namespace InteractionCollisionChannelRegistry
{
	static ECollisionChannel GChannel = ECC_WorldDynamic;

	void Set(ECollisionChannel Channel)
	{
		GChannel = Channel;
	}

	ECollisionChannel Get()
	{
		return GChannel;
	}
}
