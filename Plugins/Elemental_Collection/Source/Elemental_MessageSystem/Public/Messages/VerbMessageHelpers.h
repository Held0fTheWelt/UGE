// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "VerbMessageHelpers.generated.h"

struct FGameplayCueParameters;
struct FVerbMessage;

class APlayerController;
class APlayerState;
class UObject;
struct FFrame;


UCLASS(MinimalAPI)
class UVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Elemental|Messages")
	static ELEMENTAL_MESSAGESYSTEM_API APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Elemental|Messages")
	static ELEMENTAL_MESSAGESYSTEM_API APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Elemental|Messages")
	static ELEMENTAL_MESSAGESYSTEM_API FGameplayCueParameters VerbMessageToCueParameters(const FVerbMessage& Message);

	UFUNCTION(BlueprintCallable, Category = "Elemental|Messages")
	static ELEMENTAL_MESSAGESYSTEM_API FVerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);
};
