// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/EntityCoreInformationInterface.h"
#include "Interfaces/AssetDefinitionInformationInterface.h"
#include "Structs/AssetLoaderEvents.h"
#include "CombinedBundleActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CORE_CLASSES_API UCombinedBundleActorComponent : public UActorComponent, public IAssetDefinitionInformationInterface,
	public IEntityCoreInformationInterface
{
	GENERATED_BODY()

public:	
	UCombinedBundleActorComponent(const FObjectInitializer& ObjectInitializer);

protected:
#pragma region "AssetManager"
	/** For manual batch-loads, if needed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AssetManager")
	bool bLoadPrimaryAssets;

	UPROPERTY(Transient)
	TArray<TSoftObjectPtr<UObject>> AssetsToLoad = TArray<TSoftObjectPtr<UObject>>();
	UPROPERTY(Transient)
	TArray<TSoftClassPtr<UObject>> ClassesToLoad = TArray<TSoftClassPtr<UObject>>();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AssetManager")
	bool bNoLoading = false;
#pragma endregion "AssetManager"

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = GameFeature)
	FString GameFeatureName = "GameFeatureName";

	bool IsGameFeatureIgnored() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AssetManager")
	bool bComponentHasComponentAssetToLoad = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AssetManager")
	bool bComponentHasActorAssetToLoad = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AssetManager")
	bool bDebugRoute = false;
protected:
	/** Called when the actor enters play. You may want to trigger asset loading here. */
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma region "AssetManager"
	/** Wird aufgerufen, wenn das Asset-Management-System bereit ist, die Assets zu laden */
	virtual void LoadPrimaryAssets();

	/** Wird gefeuert, wenn das Definition-Asset nicht gefunden oder geladen werden konnte */
	UFUNCTION()
	void OnDefinitionAssetNotFound(class UObject* InOwner, bool bComponentHasClassesToLoad = true);

	/** Wird gefeuert, wenn Endplay den Pawn abmeldet*/
	void UnbindAssetLoaderEventListener();

	UFUNCTION()
	virtual void SetupLoadedAsset(UObject* Owner, UObject* LoadedObject);

private:
	virtual FName GetDefinitionAssetName_Implementation() const override;
	virtual void SetDefinitionAssetName_Implementation(FName AssetName) override {}

private:
	FAssetLoaderEvents AssetLoaderEvents;
#pragma endregion "AssetManager"

private:
	UPROPERTY(Transient)
	bool bIgnoredFeatureOnThisActor = false;

private:
	void DeferredDestroy();

#pragma region "CoreInformation Editor"
public:
	virtual FOnInformationUpdated& GetOnInformationUpdated() override
	{
		return InformationUpdated;
	}

protected:
	FOnInformationUpdated InformationUpdated;
	UPROPERTY(Transient)
	bool bLogCoreInformation = false;
protected:
	virtual TArray<FInformationSet> GetSettingsVariableSets_Implementation() const override;
private:
	virtual void SetEntityCoreLoggingEnabled_Implementation(const bool bEnabled) override;
	virtual FName GetCoreInformationName_Implementation() const override;
#pragma region "CoreInformation Editor"
};
