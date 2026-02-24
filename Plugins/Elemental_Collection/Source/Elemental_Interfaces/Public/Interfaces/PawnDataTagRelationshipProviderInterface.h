// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnDataTagRelationshipProviderInterface.generated.h"

class UObject;

/**
 * Interface for PawnData (or similar) that can provide a tag relationship mapping for the ability system.
 * Implemented by Core_System (PawnData_Core); GameFeature_PawnExtension uses only this interface.
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UPawnDataTagRelationshipProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IPawnDataTagRelationshipProviderInterface
{
	GENERATED_BODY()

public:
	/** Returns the tag relationship mapping asset, or nullptr if none. Caller (e.g. PawnExtension) passes this to IAbilitySystemTagRelationshipReceiverInterface. */
	UFUNCTION(BlueprintCallable, Category = "PawnData")
	virtual UObject* GetTagRelationshipMapping() const { return nullptr; }
};
