// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/HelicopterActor.h"
#include "SM_HelicopterActor.generated.h"

/**
 * 
 */
UCLASS()
class HELICOPTERS_API ASM_HelicopterActor : public AHelicopterActor
{
	GENERATED_BODY()

public:
	ASM_HelicopterActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> Mesh;

private:
	class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
};
