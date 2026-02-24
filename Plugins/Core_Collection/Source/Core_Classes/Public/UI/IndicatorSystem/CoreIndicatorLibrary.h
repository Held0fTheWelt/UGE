// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "CoreIndicatorLibrary.generated.h"

class AController;
class UCoreIndicatorManagerComponent;
class UObject;
struct FFrame;

UCLASS()
class CORE_CLASSES_API UCoreIndicatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UCoreIndicatorLibrary();
	
	/**  */
	UFUNCTION(BlueprintCallable, Category = Indicator)
	static UCoreIndicatorManagerComponent* GetIndicatorManagerComponent(AController* Controller);
};
