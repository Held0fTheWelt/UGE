// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/InteractWidgetInterface.h"
#include "InteractUserWidgetBase.generated.h"

/**
 * 
 */
UCLASS(editinlinenew, BlueprintType, Blueprintable, meta = (DontUseGenericSpawnObject = "True", DisableNativeTick))
class GAMEFEATURE_INTERACTRUNTIME_API UInteractUserWidgetBase : public UUserWidget, public IInteractWidgetInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image;

private:
	virtual void SetImage_Implementation(class UTexture2D* Texture) override;
	virtual void SetText_Implementation(const FString& String) override;
};
