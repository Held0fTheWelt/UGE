// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "GauntletTestControllerBootTest.h"

#include "CoreTestControllerBootTest.generated.h"

class UObject;

UCLASS()
class CORE_TESTS_API UCoreTestControllerBootTest : public UGauntletTestControllerBootTest
{
	GENERATED_BODY()

protected:

	// Delay so Gauntlet can attach after launch; then IsBootProcessComplete requires valid World and GameInstance (WarCollection boot).
	const double TestDelay = 20.0;

	//~UGauntletTestControllerBootTest interface
	virtual bool IsBootProcessComplete() const override;
	//~End of UGauntletTestControllerBootTest interface
};
