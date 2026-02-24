// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CommonUserWidget.h"
#include "PerformanceStats/PerformanceStatTypes.h"

#include "CorePerfStatContainerBase.generated.h"

class UObject;
struct FFrame;

/**
 * UCorePerfStatContainerBase
 *
 * Panel that contains a set of UCorePerfStatWidgetBase widgets and manages
 * their visibility based on user settings.
 */
UCLASS(Abstract)
class CORE_CLASSES_API UCorePerfStatContainerBase : public UCommonUserWidget
{
public:
	UCorePerfStatContainerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	GENERATED_BODY()

	//~UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~End of UUserWidget interface

	UFUNCTION(BlueprintCallable)
	void UpdateVisibilityOfChildren();

protected:
	// Are we showing text or graph stats?
	UPROPERTY(EditAnywhere, Category=Display)
	ECoreStatDisplayMode StatDisplayModeFilter = ECoreStatDisplayMode::TextAndGraph;
};
