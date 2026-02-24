// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CombinedBundleActor.h"
#include "SM_CombinedActor.generated.h"

/**
 * 
 */
UCLASS()
class CORE_CLASSES_API ASM_CombinedActor : public ACombinedBundleActor
{
	GENERATED_BODY()
public:
	ASM_CombinedActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
};
