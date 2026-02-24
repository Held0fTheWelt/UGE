// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Controllers/CommonPlayerController.h"

#include "ModularPlayerController.generated.h"

class UObject;

/** Minimal class that supports extension by game feature plugins */
UCLASS(Blueprintable)
class ELEMENTAL_CLASSES_API AModularPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

public:
	//~ Begin AActor interface
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor interface

	//~ Begin APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	//~ End APlayerController interface
};

