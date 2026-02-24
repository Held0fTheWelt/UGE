// Copyright Epic Games, Inc. All Rights Reserved.
// Core_Collection

#include "GameModes/CoreWorldSettings.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Misc/UObjectToken.h"
#include "Logging/MessageLog.h"
#include "Engine/AssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreWorldSettings)

ACoreWorldSettings::ACoreWorldSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FPrimaryAssetId ACoreWorldSettings::GetDefaultGameplayExperience() const
{
	FPrimaryAssetId Result;
	if (!DefaultGameplayExperience.IsNull())
	{
		Result = UAssetManager::Get().GetPrimaryAssetIdForPath(DefaultGameplayExperience.ToSoftObjectPath());

		if (!Result.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("%s.DefaultGameplayExperience is %s but that failed to resolve into an asset ID (you might need to add a path to the Asset Rules in your game feature plugin or project settings"),
				*GetPathNameSafe(this), *DefaultGameplayExperience.ToString());
		}
	}
	return Result;
}

#if WITH_EDITOR
void ACoreWorldSettings::CheckForErrors()
{
	Super::CheckForErrors();

	FMessageLog MapCheck("MapCheck");

	for (TActorIterator<APlayerStart> PlayerStartIt(GetWorld()); PlayerStartIt; ++PlayerStartIt)
	{
		APlayerStart* PlayerStart = *PlayerStartIt;
		if (IsValid(PlayerStart) && PlayerStart->GetClass() == APlayerStart::StaticClass())
		{
			MapCheck.Warning()
				->AddToken(FUObjectToken::Create(PlayerStart))
				->AddToken(FTextToken::Create(FText::FromString("is a normal APlayerStart, consider using a custom PlayerStart class if needed.")));
		}
	}
}
#endif
