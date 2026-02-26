// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ControllerComponent.h"
#include "GameplayTagContainer.h"

#include "NumberPopComponent.generated.h"

class UObject;
struct FFrame;

USTRUCT(BlueprintType)
struct FNumberPopRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback|Number Pops")
	FVector WorldLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback|Number Pops")
	FGameplayTagContainer SourceTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback|Number Pops")
	FGameplayTagContainer TargetTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback|Number Pops")
	int32 NumberToDisplay = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback|Number Pops")
	bool bIsCriticalDamage = false;

	FNumberPopRequest()
		: WorldLocation(ForceInitToZero)
	{
	}
};

/**
 * Component for displaying number pops (damage, healing, etc.). Use via interface for framework communication.
 */
UCLASS(Abstract)
class GAMEFEATURE_FEEDBACKSYSTEMRUNTIME_API UNumberPopComponent : public UControllerComponent
{
	GENERATED_BODY()

public:

	UNumberPopComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Feedback|Number Pops")
	virtual void AddNumberPop(const FNumberPopRequest& NewRequest) {}
};
