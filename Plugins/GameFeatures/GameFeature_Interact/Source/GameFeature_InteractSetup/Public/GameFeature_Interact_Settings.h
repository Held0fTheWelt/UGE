// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "GameFeature_Interact_Settings.generated.h"
/**
 *
 */
UCLASS(Config = Game, defaultconfig)
class GAMEFEATURE_INTERACTSETUP_API UGameFeature_Interact_Settings : public UObject
{
	GENERATED_BODY()

public:
	UGameFeature_Interact_Settings(const FObjectInitializer& obj);

	UPROPERTY(Config, EditAnywhere, Category = Interact)
	bool bUseGlobalQueryCollisionDebug;

	UPROPERTY(Config, EditAnywhere, Category = Interact, meta = (EditCondition = "bUseGlobalQueryCollisionDebug"))
	bool bGlobalQueryCollisionDebug;

	UPROPERTY(Config, EditAnywhere, Category = Interact, meta = (HideInDetailPanel))
	TArray<TSoftClassPtr<class AActor>> DefinedCollisionDebug;

	UPROPERTY(Config, EditAnywhere, Category = Interact)
	bool bUseGlobalDefaultInteractLimit;

	UPROPERTY(Config, EditAnywhere, Category = Interact, meta = (EditCondition = "bUseGlobalDefaultInteractLimit"))
	bool bHasDefaultInteractLimit;

	UPROPERTY(Config, EditAnywhere, Category = Interact, meta = (EditCondition = "bUseGlobalDefaultInteractLimit"))
	int32 DefaultInteractLimit;
};
