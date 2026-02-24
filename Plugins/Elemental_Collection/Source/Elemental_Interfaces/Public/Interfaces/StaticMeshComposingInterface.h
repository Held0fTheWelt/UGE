// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs/MeshComponentCollection.h"
#include "StaticMeshComposingInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNotifyMeshSetupComplete, FMeshComponentCollection, ComponentCollection);
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStaticMeshComposingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API IStaticMeshComposingInterface
{
	GENERATED_BODY()

public:

public:
	virtual FNotifyMeshSetupComplete& GetOnNotifyMeshSetupComplete() = 0;
};
