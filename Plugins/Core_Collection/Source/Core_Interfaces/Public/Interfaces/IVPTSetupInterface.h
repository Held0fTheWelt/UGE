// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IVPTSetupInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UVPTSetupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Implemented by actors/components that expose GT (Gran Turismo-style) setup parameters for VPT.
 * Enables vpt.Setup.Print, vpt.Setup.Set, and vpt.Setup.ApplyDelta console commands.
 */
class CORE_INTERFACES_API IVPTSetupInterface
{
	GENERATED_BODY()

public:
	/**
	 * Read a named GT setup parameter.
	 * @param Param     Parameter name (e.g., "FrontSpringRate")
	 * @param bOutValid Set to true if the parameter exists and was read successfully
	 * @return          Parameter value, or 0.0 if not found
	 */
	virtual double GetVPTSetupParam(FName Param, bool& bOutValid) const
	{
		bOutValid = false;
		return 0.0;
	}

	/**
	 * Write a named GT setup parameter.
	 * @param Param Parameter name
	 * @param Value New value to apply
	 * @return      True if the parameter was recognized and applied
	 */
	virtual bool SetVPTSetupParam(FName Param, double Value) { return false; }

	/** Returns all GT setup parameters as a name→value snapshot (for vpt.Setup.Print) */
	virtual TMap<FName, double> GetVPTSetupSnapshot() const { return {}; }
};

static IVPTSetupInterface* FindSetupInterface(AActor* Target)
{
	if (!Target) return nullptr;
	if (Target->Implements<UVPTSetupInterface>()) return Cast<IVPTSetupInterface>(Target);
	TArray<UActorComponent*> Comps = Target->GetComponentsByInterface(UVPTSetupInterface::StaticClass());
	return Comps.Num() > 0 ? Cast<IVPTSetupInterface>(Comps[0]) : nullptr;
}
