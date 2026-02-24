// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Subsystems/AttributeValueByTagSubsystem.h"
#include "AttributeValueByTagSubsystemRegistry.generated.h"

/**
 * World subsystem registry for the concrete UAttributeValueByTagSubsystem implementation.
 * GameFeature_HealthSystem registers its UHealthAttributeValueByTagSubsystem here;
 * GameFeature_AnimationSystem (and others) resolve the subsystem via this registry
 * so they never depend on Core or GameFeature_HealthSystem.
 */
UCLASS()
class ELEMENTAL_INTERFACES_API UAttributeValueByTagSubsystemRegistry : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Returns the registered attribute-by-tag subsystem, or nullptr if none registered. */
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	UAttributeValueByTagSubsystem* GetSubsystem() const { return Subsystem; }

	/** Registers the concrete implementation (e.g. from GameFeature_HealthSystem). Called by the implementing module. */
	void SetSubsystem(UAttributeValueByTagSubsystem* InSubsystem);

private:
	UPROPERTY()
	TObjectPtr<UAttributeValueByTagSubsystem> Subsystem;
};
