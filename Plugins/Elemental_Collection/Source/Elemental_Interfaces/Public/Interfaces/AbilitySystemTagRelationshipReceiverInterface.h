// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilitySystemTagRelationshipReceiverInterface.generated.h"

class UObject;

/**
 * Interface for an AbilitySystemComponent that can receive a tag relationship mapping.
 * Implemented by Core_AbilitySystem (UCoreAbilitySystemComponent); GameFeature_PawnExtension uses only this interface.
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UAbilitySystemTagRelationshipReceiverInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IAbilitySystemTagRelationshipReceiverInterface
{
	GENERATED_BODY()

public:
	/** Sets the tag relationship mapping (e.g. from PawnData). Mapping is the raw UObject* from IPawnDataTagRelationshipProviderInterface::GetTagRelationshipMapping(). */
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	virtual void SetTagRelationshipMapping(UObject* Mapping) {}
};
