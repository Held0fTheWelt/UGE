// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/GameEngine_Core.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameEngine_Core)

class IEngineLoop;

UGameEngine_Core::UGameEngine_Core(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGameEngine_Core::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);
}