// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"

#include "ModularAIController.generated.h"

class UObject;

/** Minimal class that supports extension by game feature plugins */
UCLASS( Blueprintable)
class ELEMENTAL_CLASSES_API AModularAIController : public AAIController
{
	GENERATED_BODY()

public:
	//~ Begin AActor Interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor Interface
};
