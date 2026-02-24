// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DebugCameraController.h"
#include "DebugCameraController_Elemental.generated.h"

/** Used for controlling the debug camera when it is enabled via the cheat manager.
 */
UCLASS()
class ELEMENTAL_PLAYER_API ADebugCameraController_Elemental : public ADebugCameraController
{
	GENERATED_BODY()

public:

	ADebugCameraController_Elemental(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void AddCheats(bool bForce) override;
};
