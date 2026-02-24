// Fill out your copyright notice in the Description page of Project Settings.


#include "Session/GameSession_Core.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameSession_Core)

AGameSession_Core::AGameSession_Core(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool AGameSession_Core::ProcessAutoLogin()
{
	// This is actually handled in LyraGameMode::TryDedicatedServerLogin
	return true;
}

void AGameSession_Core::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AGameSession_Core::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}