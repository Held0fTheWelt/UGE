// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/CombinedWheeledVehiclePawn.h"

#include "VehiclePawn.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_VehiclePawn, Log, All);

/**
 * 
 */
UCLASS()
class VEHICLES_API AVehiclePawn : public ACombinedWheeledVehiclePawn
{
	GENERATED_BODY()
	
public:
	AVehiclePawn();
	
protected:
	virtual void BeginPlay() override;;

};
