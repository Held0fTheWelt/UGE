// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CombinedBundleActor.h"
#include "Interfaces/InteractDummyInterface.h"
#include "DummyActor.generated.h"

/**
 * 
 */
UCLASS()
class CORE_CLASSES_API ADummyActor : public ACombinedBundleActor, public IInteractDummyInterface
{
	GENERATED_BODY()
	
public:
	ADummyActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> Root;
};
