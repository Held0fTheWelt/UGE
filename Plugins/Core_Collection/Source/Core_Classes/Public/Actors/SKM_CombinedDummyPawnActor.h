// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/DummyActor.h"
#include "SKM_CombinedDummyPawnActor.generated.h"

/**
 * 
 */
UCLASS()
class CORE_CLASSES_API ASKM_CombinedDummyPawnActor : public ADummyActor
{
	GENERATED_BODY()

public:
	ASKM_CombinedDummyPawnActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class USkeletalMeshComponent> Mesh;

protected:
	virtual void BeginPlay() override;

	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;

};
