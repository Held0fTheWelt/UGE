// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/CombinedBundlePawn.h"
#include "Interfaces/OccupiableInterface.h"
#include "Interfaces/AircraftSetupInterface.h"
#include "AircraftPawn.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_AircraftPawn, Log, All);

/**
 * 
 */
UCLASS()
class AIRCRAFTS_API AAircraftPawn : public ACombinedBundlePawn, 
	public IAircraftSetupInterface
{
	GENERATED_BODY()

public:
	AAircraftPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class UAudioComponent> EngineSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class UFloatingPawnMovement> AircraftMovementComponent;

private:
	virtual void SetSpeed_Implementation(float Speed) override;
};
