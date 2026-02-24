// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Components/ControllerComponent.h"

#include "CoreFrontendStateComponent.generated.h"

class AController;
class UObject;
struct FFrame;

/**
 * UCoreFrontendStateComponent
 *
 * Component for managing frontend state in Core.
 * Adapted from ULyraFrontendStateComponent.
 */
UCLASS(BlueprintType, Blueprintable)
class CORE_CLASSES_API UCoreFrontendStateComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UCoreFrontendStateComponent(const FObjectInitializer& ObjectInitializer);

	static UCoreFrontendStateComponent* GetComponent(AController* Controller);

	UFUNCTION(BlueprintCallable, Category = "Core|Frontend")
	void SetIsInFrontend(bool bInFrontend);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Core|Frontend")
	bool IsInFrontend() const { return bIsInFrontend; }

	DECLARE_EVENT_OneParam(UCoreFrontendStateComponent, FFrontendStateChangedEvent, bool bInFrontend)
	FFrontendStateChangedEvent OnFrontendStateChanged;

private:
	UPROPERTY()
	bool bIsInFrontend = false;
};
