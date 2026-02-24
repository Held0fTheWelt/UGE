// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/AircraftPawn.h"
#include "SM_AircraftPawn.generated.h"

/**
 * 
 */
UCLASS()
class AIRCRAFTS_API ASM_AircraftPawn : public AAircraftPawn
{
	GENERATED_BODY()

public:
	ASM_AircraftPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
};
