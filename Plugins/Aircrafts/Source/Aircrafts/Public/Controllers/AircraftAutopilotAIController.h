// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AircraftAutopilotAIController.generated.h"

/**
 * 
 */
UCLASS()
class AIRCRAFTS_API AAircraftAutopilotAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void Tick(float DeltaTime) override;
};
