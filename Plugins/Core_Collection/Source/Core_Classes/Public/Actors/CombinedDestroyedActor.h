// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombinedDestroyedActor.generated.h"

UCLASS()
class CORE_CLASSES_API ACombinedDestroyedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACombinedDestroyedActor();

protected:
	virtual void BeginPlay() override;

};
