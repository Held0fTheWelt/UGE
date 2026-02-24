// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/SubmarineActor.h"
#include "SM_SubmarineActor.generated.h"

/**
 * 
 */
UCLASS()
class SHIPS_API ASM_SubmarineActor : public ASubmarineActor
{
	GENERATED_BODY()
public:
	ASM_SubmarineActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
};
