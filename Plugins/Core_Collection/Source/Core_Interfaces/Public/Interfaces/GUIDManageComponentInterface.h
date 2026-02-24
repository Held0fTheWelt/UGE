// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GUIDManageComponentInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGUIDManageComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CORE_INTERFACES_API IGUIDManageComponentInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void SetDumpedGUID(const FGuid& InGuid);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	FGuid GetDumpedGUID() const;
};
