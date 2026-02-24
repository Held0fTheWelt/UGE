// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"

#include "CommonPlayerController.generated.h"

class UObject;

UCLASS(config = Game)
class ELEMENTAL_COMMON_API ACommonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACommonPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ReceivedPlayer() override;
	virtual void SetPawn(class APawn* InPawn) override;
	virtual void OnPossess(class APawn* APawn) override;
	virtual void OnUnPossess() override;

protected:
	virtual void OnRep_PlayerState() override;
};

#undef UE_API
