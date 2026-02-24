// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombinedBundleUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class CORE_CLASSES_API UCombinedBundleUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	TArray<TSoftObjectPtr<UObject>> AssetsToLoad = TArray<TSoftObjectPtr<UObject>>();
	UPROPERTY(Transient)
	TArray<TSoftClassPtr<UObject>> ClassesToLoad = TArray<TSoftClassPtr<UObject>>();

protected:
	virtual void NativePreConstruct() override;
	UFUNCTION()
	virtual void AllAsyncAssetsLoaded();
};
