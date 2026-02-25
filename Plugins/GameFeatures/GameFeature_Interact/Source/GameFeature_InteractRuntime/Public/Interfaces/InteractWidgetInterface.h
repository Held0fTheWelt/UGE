// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractWidgetInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEFEATURE_INTERACTRUNTIME_API IInteractWidgetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void SetImage(class UTexture2D* Texture);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Default)
	void  SetText(const FString& String);
};
