// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "ExperienceDefinition.generated.h"

class UGameFeatureAction;
class UObject;
class UExperienceActionSet;

/**
 * Definition of an experience. Used via IExperienceManagerInterface from Elemental.
 */
UCLASS(BlueprintType, Const)
class CORE_DATA_API UExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UExperienceDefinition();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif

public:
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	/** The default pawn data to use for players (path resolved via GetDefaultPawnDataPathFromCurrentExperience). */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<const UObject> DefaultPawnData;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<TObjectPtr<UExperienceActionSet>> ActionSets;
};
