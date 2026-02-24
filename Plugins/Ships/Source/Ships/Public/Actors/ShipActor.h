// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CombinedBundleActor.h"
#include "Interfaces/OccupiableInterface.h"
//#include "Interfaces/AircraftSetupInterface.h"
#include "ShipActor.generated.h"

/**
 *
 */
UCLASS()
class SHIPS_API AShipActor : public ACombinedBundleActor,
	//public IAircraftSetupInterface,
	public IOccupiableInterface
{
	GENERATED_BODY()

public:
	AShipActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class UAudioComponent> EngineSound;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	//TObjectPtr<class UFloatingPawnMovement> AircraftMovementComponent;

//private:
	//virtual void SetSpeed_Implementation(float Speed) override;
};
