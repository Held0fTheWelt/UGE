// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/AircraftPawn.h"
#include "SKM_AircraftPawn.generated.h"

/**
 * 
 */
UCLASS()
class AIRCRAFTS_API ASKM_AircraftPawn : public AAircraftPawn
{
	GENERATED_BODY()
	
public:
	ASKM_AircraftPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
};
