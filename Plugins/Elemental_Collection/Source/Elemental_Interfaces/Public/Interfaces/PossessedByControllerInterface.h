// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PossessedByControllerInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnPossessedByController, class AController*, Controller);


UINTERFACE(MinimalAPI)
class UPossessedByControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API IPossessedByControllerInterface
{
	GENERATED_BODY()

public:
	/** Must be implemented by inheriting class */
	virtual FOnPawnPossessedByController& GetOnPawnPossessedByController() = 0;

	/** Optional default implementation for Blueprint event */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Possessed By Controller")
	void ControllerPossessesPawn(AController* InController);
};
