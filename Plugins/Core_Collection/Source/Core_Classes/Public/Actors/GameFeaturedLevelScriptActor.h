// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Interfaces/ShouldAddGameFeatureInterface.h"
#include "Interfaces/AssetDefinitionInformationInterface.h"
#include "GameFeaturedLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class CORE_CLASSES_API AGameFeaturedLevelScriptActor : public ALevelScriptActor, public IShouldAddGameFeatureInterface, public IAssetDefinitionInformationInterface
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual FName GetDefinitionAssetName_Implementation() const override;

#pragma region "GameFeatures"
private:
	UPROPERTY(Transient)
	bool bGameFeatureInitialized = false;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameFeature)
	TArray<FString> IgnoredGameFeatures;

	/** For manual batch-loads, if needed */
	UPROPERTY(Transient)
	TArray<FName> PrimaryAssetNamesToLoad;
protected:
	virtual bool ShouldIgnoreGameFeature_Implementation(const FString& GameFeature) const override;
private:
	bool AddGameFeatureListener();
	bool RemoveGameFeatureListener();
#pragma endregion "GameFeatures"
	
};
