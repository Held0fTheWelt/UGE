// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HelicopterAnimInstanceInterface.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentRotorMain, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyCurrentRotorTail, float, Value);
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHelicopterAnimInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELICOPTERS_API IHelicopterAnimInstanceInterface
{
	GENERATED_BODY()

public:
	virtual FNotifyCurrentRotorMain& GetNotifyCurrentRotorMain() = 0;
	virtual FNotifyCurrentRotorTail& GetNotifyCurrentRotorTail() = 0;
};
