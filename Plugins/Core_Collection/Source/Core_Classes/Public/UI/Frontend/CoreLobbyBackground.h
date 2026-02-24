// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Engine/DataAsset.h"

#include "CoreLobbyBackground.generated.h"

class UObject;
class UWorld;

/**
 * UCoreLobbyBackground
 *
 * Developer settings / editor cheats for lobby background.
 * Adapted from ULyraLobbyBackground.
 */
UCLASS(config=EditorPerProjectUserSettings)
class CORE_CLASSES_API UCoreLobbyBackground : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> BackgroundLevel;
};
