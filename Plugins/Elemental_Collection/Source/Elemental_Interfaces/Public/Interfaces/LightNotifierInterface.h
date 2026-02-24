// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LightNotifierInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyLightChange, const FName&, Identifier, float, Value);

UINTERFACE(MinimalAPI)
class ULightNotifierInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API ILightNotifierInterface
{
	GENERATED_BODY()

public:
	virtual FNotifyLightChange& GetNotifyOnLightChange() = 0;
};
