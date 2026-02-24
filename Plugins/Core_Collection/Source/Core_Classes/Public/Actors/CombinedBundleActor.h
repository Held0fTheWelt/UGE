// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/AssetDefinitionInformationInterface.h"
#include "Interfaces/ShouldAddGameFeatureInterface.h"
#include "Interfaces/SceneComponentCatchInterface.h"
#include "Interfaces/OccupiableInterface.h"
#include "Interfaces/InteractInterface.h"
#include "Interfaces/EntityCoreInformationInterface.h"
#include "Structs/AssetLoaderEvents.h"
#include "CombinedBundleActor.generated.h"

UCLASS()
class CORE_CLASSES_API ACombinedBundleActor : public AActor, 
	public IAssetDefinitionInformationInterface,
	public IShouldAddGameFeatureInterface, 
	public ISceneComponentCatchInterface,
	public IInteractInterface,
	public IEntityCoreInformationInterface
{
	GENERATED_BODY()

public:
	ACombinedBundleActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UArrowComponent* Forward;

protected:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;

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

#pragma region "AssetManager"
protected:
	/** Which definition-asset to load (overridable in Blueprint) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AssetManager")
	FName DataAssetIdentifier;

	UFUNCTION()
	virtual void SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject);

	/** Wird aufgerufen, wenn das Asset-Management-System bereit ist, die Assets zu laden */
	virtual void LoadPrimaryAssets();

	/** Wird gefeuert, wenn Endplay den Pawn abmeldet*/
	void UnbindAssetLoaderEventListener(); 

private:
	virtual FName GetDefinitionAssetName_Implementation() const override;
	virtual void SetDefinitionAssetName_Implementation(FName AssetName) override;

private:
	FAssetLoaderEvents AssetLoaderEvents;
#pragma endregion "AssetManager"

#pragma region "Interact"
public:
	virtual FOnInteractWithPawn& GetOnInteractWithPawn() override
	{
		return OnInteractWithPawn;
	}

	virtual FOnStopInteractingWithPawn& GetOnStopInteractingWithPawn() override
	{
		return OnStopInteractingWithPawn;
	}
private:
	FOnInteractWithPawn OnInteractWithPawn;
	FOnStopInteractingWithPawn OnStopInteractingWithPawn;
private:
	UFUNCTION()
	void InteractedWithPawn(AActor* Interactor, AController* InteractorInstigator);
	UFUNCTION()
	void StoppedInteractWithPawn(AActor* Interactor, AController* InteractorInstigator);
#pragma endregion "Interact"

#pragma region "CoreInformation Editor"
public:
	virtual FOnInformationUpdated& GetOnInformationUpdated() override
	{
		return InformationUpdated;
	}

private:
	FOnInformationUpdated InformationUpdated;
private:
	virtual void SetEntityCoreLoggingEnabled_Implementation(const bool bEnabled) override;
	virtual FName GetCoreInformationName_Implementation() const override;
	virtual TArray<FInformationSet> GetSettingsVariableSets_Implementation() const override;
private:
	UPROPERTY(Transient)
	bool bLogCoreInformation = false;
#pragma region "CoreInformation Editor"
};
