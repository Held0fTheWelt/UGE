// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "CoreTestControllerBootTest.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreTestControllerBootTest)

bool UCoreTestControllerBootTest::IsBootProcessComplete() const
{
	static double StartTime = FPlatformTime::Seconds();
	const double TimeSinceStart = FPlatformTime::Seconds() - StartTime;

	// Delay so Gauntlet can attach after launch; then require valid world and game instance (WarCollection boot).
	if (TimeSinceStart >= TestDelay)
	{
		const UWorld* World = GetWorld();
		if (World && World->GetGameInstance())
		{
			return true;
		}
	}

	return false;
}
