// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/CombinedBundlePawn.h"
#include "Interfaces/OccupiableInterface.h"
//#include "Interfaces/AircraftSetupInterface.h"
#include "ShipPawn.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_ShipPawn, Log, All);

/**
 * 
 */
UCLASS(abstract)
class SHIPS_API AShipPawn : public ACombinedBundlePawn
	//public IAircraftSetupInterface
{
	GENERATED_BODY()

public:
	AShipPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class UAudioComponent> EngineSound;

private:
	//virtual void SetSpeed_Implementation(float Speed) override;
};
