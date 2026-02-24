// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "GameSession_Core.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class CORE_SYSTEM_API AGameSession_Core : public AGameSession
{
	GENERATED_BODY()

public:

	AGameSession_Core(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	/** Override to disable the default behavior */
	virtual bool ProcessAutoLogin() override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
};
