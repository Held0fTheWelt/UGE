// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Structs/InformationSet.h"
#include "CoreInformationFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTAL_STRUCTURE_API UCoreInformationFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FInformationSet GetInformationSet(const FString& Category, const FString& Name, const FString& Description);

	static void AddInformationValue(FInformationSet& InformationSet, const FString& VisibleName, const FString& Value, EValueType ValueType);
};
