// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/AircraftActor.h"
#include "SM_AircraftActor.generated.h"

/**
 * 
 */
UCLASS()
class AIRCRAFTS_API ASM_AircraftActor : public AAircraftActor
{
	GENERATED_BODY()
	
public:
	ASM_AircraftActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
};
