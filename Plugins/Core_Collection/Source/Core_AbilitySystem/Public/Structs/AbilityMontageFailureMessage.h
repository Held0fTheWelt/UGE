#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityMontageFailureMessage.generated.h"

/** Failure reason that can be used to play an animation montage when a failure occurs */
USTRUCT(BlueprintType)
struct FAbilityMontageFailureMessage
{
	GENERATED_BODY()

public:
	// Player controller that failed to activate the ability, if the AbilitySystemComponent was player owned
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class APlayerController> PlayerController = nullptr;

	// Avatar actor that failed to activate the ability
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class AActor> AvatarActor = nullptr;

	// All the reasons why this ability has failed
	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer FailureTags;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> FailureMontage = nullptr;
};