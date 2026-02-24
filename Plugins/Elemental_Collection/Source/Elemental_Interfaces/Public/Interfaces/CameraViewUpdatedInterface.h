// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/LocalDefinition.h"
#include "CameraViewUpdatedInterface.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraViewUpdated, const FVector&, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraSetupChanged, ELocalDefinition, NewLocalDefinition);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCameraViewUpdatedInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API ICameraViewUpdatedInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	ELocalDefinition GetCurrentLocalDefinition() const;
	/** Must be implemented by inheriting class */
	virtual FOnCameraViewUpdated& GetCameraViewUpdated() = 0;
	virtual FOnCameraSetupChanged& GetCameraSetupChanged() = 0;
};
