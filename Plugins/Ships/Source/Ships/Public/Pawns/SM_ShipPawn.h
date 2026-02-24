// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/ShipPawn.h"
#include "SM_ShipPawn.generated.h"

/**
 * 
 */
UCLASS()
class SHIPS_API ASM_ShipPawn : public AShipPawn
{
	GENERATED_BODY()
	
	
public:
	ASM_ShipPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
	
};
