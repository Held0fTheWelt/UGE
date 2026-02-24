// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractionQuery.generated.h"

class AActor;
class UObject;
struct FFrame;

/**
 * FInteractionQuery
 *
 *	Query parameters for finding interaction options.
 *	Adapted from FLyraInteractionQuery.
 */
USTRUCT(BlueprintType)
struct CORE_INTERACT_API FInteractionQuery
{
	GENERATED_BODY()

public:
	FInteractionQuery()
		: RequestingAvatar(nullptr)
		, RequestingController(nullptr)
	{
	}

	// The requesting pawn.
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> RequestingAvatar;

	// Allow us to specify a controller - does not need to match the owner of the requesting avatar.
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AController> RequestingController;

	// A generic UObject to shove in extra data required for the interaction
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<UObject> OptionalObjectData;
};
