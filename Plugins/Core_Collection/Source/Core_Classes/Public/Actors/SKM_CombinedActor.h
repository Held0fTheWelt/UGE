// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CombinedBundleActor.h"
#include "SKM_CombinedActor.generated.h"

/**
 * 
 */
UCLASS()
class CORE_CLASSES_API ASKM_CombinedActor : public ACombinedBundleActor
{
	GENERATED_BODY()

public:
	ASKM_CombinedActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
};
