// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs/InformationSet.h"
#include "EntityCoreInformationInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInformationUpdated, const TArray<FInformationSet>&, InformationSet);

UINTERFACE(MinimalAPI)
class UEntityCoreInformationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class ELEMENTAL_INTERFACES_API IEntityCoreInformationInterface
{
	GENERATED_BODY()

public:
	virtual FOnInformationUpdated& GetOnInformationUpdated() = 0;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void SetEntityCoreLoggingEnabled(const bool bEnabled);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	FName GetCoreInformationName() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	TArray<FInformationSet> GetSettingsVariableSets() const;
};