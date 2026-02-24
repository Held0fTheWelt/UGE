// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StreamableManager.h"
#include "Enums/CollectionSpawnType.h"
#include "Interfaces/AssetDefinitionInformationInterface.h"
#include "Structs/AssetLoaderEvents.h"
#include "Structs/EntityDefinition.h"
#include "DefinedCollectionSpawningActor.generated.h"

UCLASS(HideCategories = (Rendering, Cooking, Collision, Input, HLOD, Physics))
class CORE_CLASSES_API ADefinedCollectionSpawningActor : public AActor, public IAssetDefinitionInformationInterface
{
	GENERATED_BODY()
	
public:	
	ADefinedCollectionSpawningActor();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:


#pragma region 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	FEntityDefinition DefinedActorClassName;
#pragma endregion "AssetManager"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	ECollectionSpawnType SpawnType;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	TSoftObjectPtr<class UActorCollectionDataAsset> DefinedCollection;
#endif

private:
	
	void AddCollectionToSpawningSystem();

#pragma region "AssetManager"
	/** Wird aufgerufen, wenn das Asset-Management-System bereit ist, die Assets zu laden */
	virtual void LoadPrimaryAssets();

	/** Wird für jedes einzeln geladene DataAsset gefeuert */
	UFUNCTION()
	void OnPrimaryAssetLoaded(class UObject* InOwner, class UObject* LoadedObject);

	/** Wird gefeuert, wenn Endplay den Pawn abmeldet*/
	void UnbindAssetLoaderEventListener();
	#pragma endregion "AssetManager"

	void LoadDefinedActor(class UActorCollectionDataAsset* Asset);
	void SpawnDefinedActor(class UActorCollectionDataAsset* Asset);

	FName GetDefinitionAssetName_Implementation() const;
	virtual void SetDefinitionAssetName_Implementation(FName AssetName) override {}

private:
	UPROPERTY()
	TObjectPtr<UChildActorComponent> DefinedActor;
	// Handle damit wir den Lade-Vorgang ggf. abbrechen oder den Status prüfen können
	TSharedPtr<FStreamableHandle> StreamHandle;

	FAssetLoaderEvents AssetLoaderEvents;
};
