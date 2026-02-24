// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EntityCoreEditorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ENTITYCOREEDITOR_API UEntityCoreEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	static FText GetAssetNameFromInternalNameString(const FString& InternalName);
};
