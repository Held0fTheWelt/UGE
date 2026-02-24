// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "AttributeValueByTagSubsystem.generated.h"

class UAbilitySystemComponent;

/**
 * World subsystem for resolving GAS attribute values by gameplay tag.
 * Base implementation returns 0; GameFeature_HealthSystem (or other modules that may use Core) subclass and override.
 * GameFeature_AnimationSystem uses only this base class and does not depend on Core_Collection.
 */
UCLASS(Abstract)
class ELEMENTAL_INTERFACES_API UAttributeValueByTagSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Returns the current value of the attribute identified by the tag, or 0 if unknown. Override in subclasses (e.g. Health system). */
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	virtual float GetAttributeValue(UAbilitySystemComponent* ASC, FGameplayTag AttributeTag) const { return 0.f; }
};
