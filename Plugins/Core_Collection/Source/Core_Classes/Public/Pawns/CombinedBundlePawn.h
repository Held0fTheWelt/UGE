// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/AssetDefinitionInformationInterface.h"
#include "Interfaces/Control/PawnControlInterface.h"
#include "Interfaces/EntityCoreInformationInterface.h"
#include "Interfaces/InteractInterface.h"
#include "Interfaces/OccupiableInterface.h"
#include "Interfaces/PossessedByControllerInterface.h"
#include "Interfaces/SceneComponentCatchInterface.h"
#include "Interfaces/ScenePersistentPawnInterface.h"
#include "Interfaces/ShouldAddGameFeatureInterface.h"
#include "Interfaces/SpawnPointInformationInterface.h"
#include "Structs/AssetLoaderEvents.h"
#include "CombinedBundlePawn.generated.h"

UCLASS()
class CORE_CLASSES_API ACombinedBundlePawn : public APawn,
	public IAssetDefinitionInformationInterface,
	public IPawnControlInterface,
	public ISceneComponentCatchInterface, 
	public IShouldAddGameFeatureInterface,
	public IPossessedByControllerInterface,
	public IOccupiableInterface,
	public ISpawnPointInformationInterface,
	public IInteractInterface,
	public IScenePersistentPawnInterface,
	public IEntityCoreInformationInterface
{
	GENERATED_BODY()

public:
	ACombinedBundlePawn();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UArrowComponent* Forward;
	
	UPROPERTY(EditAnywhere, Category = "Manage Load | Spawn", meta = (MakeEditWidget))
	TArray<FTransform> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Manage Load | Despawn")
	bool bActorDespawnsOnPossessOtherPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Load)
	int32 MaxActorLoad = 1;

protected:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual class USceneComponent* GetSceneComponent_Implementation(const FName& ComponentName) const override;
	void SpawnDefaultPawn();
private:
	virtual FTransform GetSpawnPointForController_Implementation(AController* Controller) const override;
	virtual bool ShouldDespawnOnUnpossess_Implementation() const { return bActorDespawnsOnPossessOtherPawn; }
	virtual int32 AddReferencedPawnToLoad_Implementation(class AActor* ActorToAdd) override
	{
		if (ActorToAdd)
		{
			for (int32 i = 0; i < LoadListActors.Num(); i++)
			{
				if (LoadListActors[i] == nullptr)
				{
					LoadListActors[i] = ActorToAdd; // Replace null reference with the new actor
					return i; // Return the index of the replaced actor
				}
			}

			if (LoadListActors.Num() >= MaxActorLoad)
			{
				return -1; // Return the index of the newly added actor
			}

			if (!LoadListActors.Contains(ActorToAdd))
			{
				return LoadListActors.Add(ActorToAdd);
			}
		}
		return -1;
	}
	virtual class AActor* GetReferencedPawnFromLoad_Implementation(const int32& Index) override
	{
		if (LoadListActors.IsValidIndex(Index))
		{
			AActor* Actor = LoadListActors[Index];
			LoadListActors[Index] = nullptr; // Clear the reference to allow garbage collection
			return Actor;
		}
		return nullptr;

	}

private:
	UPROPERTY(Transient)
	TArray<class AActor*> LoadListActors = TArray<class AActor*>();

private:
	UPROPERTY(Transient)
	bool bGameFeatureInitialized = false;


#pragma region "GameFeatures"
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameFeature)
	TArray<FString> IgnoredGameFeatures;
protected:
	virtual bool ShouldIgnoreGameFeature_Implementation(const FString& GameFeature) const override;
private:
	bool AddGameFeatureListener();
	bool RemoveGameFeatureListener();
#pragma endregion "GameFeatures"

#pragma region "AssetManager"
protected:
	/** Which definition-asset to load (overridable in Blueprint) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AssetManager")
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

#pragma region "Possess"
public:
	virtual FOnPawnPossessedByController& GetOnPawnPossessedByController() override
	{
		return OnPossessedByController;
	}

private:
	FOnPawnPossessedByController OnPossessedByController;

	virtual void ControllerPossessesPawn_Implementation(AController* InController) override;
#pragma endregion "Possess"

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
protected:
	virtual TArray<FInformationSet> GetSettingsVariableSets_Implementation() const override;
private:
	UPROPERTY(Transient)
	bool bLogCoreInformation = false;
#pragma region "CoreInformation Editor"
};
