// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SceneHierarchyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USceneHierarchyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENTAL_INTERFACES_API ISceneHierarchyInterface
{
	GENERATED_BODY()

public:
	/** 
	 * Returns the root component of the scene hierarchy.
	 * @return The root component of the scene hierarchy.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	TMap<FName, class USceneComponent*> GetSceneComponents() const;
};
