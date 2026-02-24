// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "Core_Settings.generated.h"
/**
 *
 */
UCLASS(config = CoreSettings)
class UCore_Settings : public UObject
{
	GENERATED_BODY()

public:
	UCore_Settings(const FObjectInitializer& obj) : Super(obj)
	{
		DefaultGameFeatureClasses = TArray<TSoftClassPtr<class AActor>>();
	}

	/** Default game feature classes to load at startup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Game Features")
	TArray<TSoftClassPtr<class AActor>> DefaultGameFeatureClasses;

};
