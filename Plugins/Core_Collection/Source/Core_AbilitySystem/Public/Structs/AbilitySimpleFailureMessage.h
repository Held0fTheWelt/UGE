// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

#include "AbilitySimpleFailureMessage.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ABILITY_SIMPLE_FAILURE_MESSAGE);

USTRUCT(BlueprintType)
struct CORE_ABILITYSYSTEM_API FAbilitySimpleFailureMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer FailureTags;

	UPROPERTY(BlueprintReadWrite)
	FText UserFacingReason;
};
