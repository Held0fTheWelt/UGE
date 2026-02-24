// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"

#include "ICoreActorIndicatorWidget.generated.h"

class AActor;
class UCoreIndicatorDescriptor;

UINTERFACE(MinimalAPI, BlueprintType)
class UCoreActorIndicatorWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class CORE_CLASSES_API ICoreActorIndicatorWidgetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Indicator")
	void BindIndicator(UCoreIndicatorDescriptor* Indicator);

	UFUNCTION(BlueprintNativeEvent, Category = "Indicator")
	void UnbindIndicator(const UCoreIndicatorDescriptor* Indicator);
};
