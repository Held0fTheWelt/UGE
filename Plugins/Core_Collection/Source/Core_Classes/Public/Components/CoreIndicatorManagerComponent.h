// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Components/ControllerComponent.h"

#include "CoreIndicatorManagerComponent.generated.h"

class AController;
class UCoreIndicatorDescriptor;
class UObject;
struct FFrame;

/**
 * UCoreIndicatorManagerComponent
 *
 *	Component for managing UI indicators in Core.
 *	Adapted from LyraIndicatorManagerComponent.
 */
UCLASS(BlueprintType, Blueprintable)
class CORE_CLASSES_API UCoreIndicatorManagerComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UCoreIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer);

	static UCoreIndicatorManagerComponent* GetComponent(AController* Controller);

	UFUNCTION(BlueprintCallable, Category = "Core|Indicator")
	void AddIndicator(UCoreIndicatorDescriptor* IndicatorDescriptor);
	
	UFUNCTION(BlueprintCallable, Category = "Core|Indicator")
	void RemoveIndicator(UCoreIndicatorDescriptor* IndicatorDescriptor);

	DECLARE_EVENT_OneParam(UCoreIndicatorManagerComponent, FIndicatorEvent, UCoreIndicatorDescriptor* Descriptor)
	FIndicatorEvent OnIndicatorAdded;
	FIndicatorEvent OnIndicatorRemoved;

	const TArray<UCoreIndicatorDescriptor*>& GetIndicators() const { return Indicators; }

private:
	UPROPERTY()
	TArray<TObjectPtr<UCoreIndicatorDescriptor>> Indicators;
};
