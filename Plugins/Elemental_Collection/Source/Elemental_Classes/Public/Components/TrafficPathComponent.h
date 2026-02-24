// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "TrafficPathComponent.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_CLASSES_API UTrafficPathComponent : public USplineComponent
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic Path", Transient)
	TObjectPtr<class USplineComponent> InputSplineComponent;
};
