// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "ExperienceActionSet.generated.h"

class UGameFeatureAction;

/**
 * Definition of a set of actions to perform as part of entering an experience.
 */
UCLASS(BlueprintType, NotBlueprintable)
class CORE_DATA_API UExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UExperienceActionSet();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

	UPROPERTY(EditAnywhere, Category = "Feature Dependencies")
	TArray<FString> GameFeaturesToEnable;
};
