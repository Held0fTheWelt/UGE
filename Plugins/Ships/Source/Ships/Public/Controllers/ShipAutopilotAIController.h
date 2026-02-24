// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShipAutopilotAIController.generated.h"

/**
 *
 */
UCLASS()
class SHIPS_API AShipAutopilotAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void Tick(float DeltaTime) override;
};
