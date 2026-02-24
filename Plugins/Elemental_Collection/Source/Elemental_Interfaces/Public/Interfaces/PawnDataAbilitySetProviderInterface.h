// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnDataAbilitySetProviderInterface.generated.h"

class UAbilitySystemComponent;
class UObject;

/**
 * Interface for pawn data (or similar assets) that can grant ability sets to an ability system component.
 * Core_Collection implements this on UPawnData_Core; Elemental only uses the interface so the hierarchy stays clean.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UPawnDataAbilitySetProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENTAL_INTERFACES_API IPawnDataAbilitySetProviderInterface
{
	GENERATED_BODY()

public:
	/** Returns the ability set assets to grant (e.g. UCoreAbilitySet). Used by applier registered in Core_Interfaces so Core_System does not depend on Core_AbilitySystem. */
	virtual TArray<UObject*> GetAbilitySetsToGrant() const { return {}; }

	/** Grant all ability sets from this pawn data to the given ASC. SourceObject is typically the owner (e.g. PlayerState). Default implementation invokes the applier from Core_Interfaces. */
	virtual void GrantAbilitySetsToAbilitySystem(UAbilitySystemComponent* ASC, UObject* SourceObject) = 0;
};
