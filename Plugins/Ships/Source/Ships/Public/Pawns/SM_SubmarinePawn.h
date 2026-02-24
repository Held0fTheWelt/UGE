// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/SubmarinePawn.h"
#include "SM_SubmarinePawn.generated.h"

/**
 * 
 */
UCLASS()
class SHIPS_API ASM_SubmarinePawn : public ASubmarinePawn
{
	GENERATED_BODY()
public:
	ASM_SubmarinePawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
};
