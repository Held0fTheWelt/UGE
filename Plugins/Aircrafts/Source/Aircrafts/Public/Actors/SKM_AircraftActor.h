// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/AircraftActor.h"
#include "SKM_AircraftActor.generated.h"

/**
 * 
 */
UCLASS()
class AIRCRAFTS_API ASKM_AircraftActor : public AAircraftActor
{
	GENERATED_BODY()
	
public:
	ASKM_AircraftActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;

};
