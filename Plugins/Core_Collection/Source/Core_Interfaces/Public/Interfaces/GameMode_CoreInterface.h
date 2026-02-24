// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameMode_CoreInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameMode_CoreInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CORE_INTERFACES_API IGameMode_CoreInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void SpawnNewActorAtLocation(class AController* Instigator, const FTransform& InTransform);
};
