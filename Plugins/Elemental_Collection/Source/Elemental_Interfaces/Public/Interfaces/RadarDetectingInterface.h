// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/SoftObjectPath.h"
#include "RadarDetectingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URadarDetectingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API IRadarDetectingInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void NotifyAttackOver(const FSoftObjectPath& HomingTarget);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void NotifyAttack(const FSoftObjectPath& AttackActor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void NotifyDeath(const FSoftObjectPath& DeadActor);


};
