// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ShipActor.h"
#include "SM_ShipActor.generated.h"

/**
 * 
 */
UCLASS()
class SHIPS_API ASM_ShipActor : public AShipActor
{
	GENERATED_BODY()

public:
	ASM_ShipActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
};
