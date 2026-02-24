// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "GameEngine_Core.generated.h"

/**
 * 
 */
UCLASS()
class CORE_SYSTEM_API UGameEngine_Core : public UGameEngine
{
	GENERATED_BODY()
	
public:

	UGameEngine_Core(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void Init(class IEngineLoop* InEngineLoop) override;
};
