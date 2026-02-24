// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnPointInformationInterface.generated.h"

UINTERFACE(MinimalAPI)
class USpawnPointInformationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API ISpawnPointInformationInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces | Register")
	FTransform GetSpawnPointForController(AController* Controller) const;
};
