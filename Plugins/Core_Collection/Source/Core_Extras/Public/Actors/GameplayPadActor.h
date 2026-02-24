// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CombinedBundleActor.h"
#include "GameplayPadActor.generated.h"

/**
 * 
 */
UCLASS()
class CORE_EXTRAS_API AGameplayPadActor : public ACombinedBundleActor
{
	GENERATED_BODY()
	
public:
	AGameplayPadActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class UBoxComponent> Volume;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class UTextRenderComponent> Text;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class UStaticMeshComponent> PadMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class UStaticMeshComponent> PadBase;
};
