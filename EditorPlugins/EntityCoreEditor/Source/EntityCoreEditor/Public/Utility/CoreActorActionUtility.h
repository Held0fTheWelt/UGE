// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorActionUtility.h"
#include "CoreActorActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class ENTITYCOREEDITOR_API UCoreActorActionUtility : public UActorActionUtility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Utility")
	void OpenActorEditor();
};
