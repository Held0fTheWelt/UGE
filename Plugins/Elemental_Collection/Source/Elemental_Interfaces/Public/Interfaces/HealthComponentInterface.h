// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthComponentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UHealthComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API IHealthComponentInterface
{
	GENERATED_BODY()

public:
	virtual void DamageSelfDestruct() = 0;

	/** Returns current health value. Used by Core (e.g. RPC/vitals) without depending on GameFeature_HealthSystem. */
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float GetHealth() const { return 0.f; }
};
