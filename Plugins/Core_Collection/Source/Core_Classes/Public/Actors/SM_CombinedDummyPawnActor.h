// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/DummyActor.h"
#include "SM_CombinedDummyPawnActor.generated.h"

UCLASS()
class CORE_CLASSES_API ASM_CombinedDummyPawnActor : public ADummyActor
{
	GENERATED_BODY()

public:
	ASM_CombinedDummyPawnActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class UStaticMeshComponent> Mesh;

protected:
	virtual void BeginPlay() override;

	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
};
