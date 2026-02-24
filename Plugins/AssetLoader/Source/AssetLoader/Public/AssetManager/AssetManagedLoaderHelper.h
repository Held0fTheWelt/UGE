// AssetManagedLoaderHelper.h
/*
 * Copyright (c) 2024 Yves Tanas
 * All Rights Reserved.
 *
 * This file is part of the Collections project.
 *
 * Unauthorized copying of this file, via any medium, is strictly prohibited.
 * Proprietary and confidential.
 *
 * This software may be used only as expressly authorized by the copyright holder.
 * Unless required by applicable law or agreed to in writing, software distributed
 * under this license is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 *
 * For licensing inquiries, please contact: yves.tanas@example.com
 *
 * Contributors:
 *   Yves Tanas <yves.tanas@example.com>
 *
 * -------------------------------------------------------------------------------
 * File:        AssetManagedLoaderHelper.h
 * Created:     2025-06-16
 * Description: Declaration of UAssetManagedLoaderHelper—a fully static helper that:
 *                1) Asynchronously loads a single "definition" PrimaryDataAsset by name
 *                2) On success, matches a given UClass against its selectors
 *                3) Broadcasts a pre‐load event with the list of derived AssetDefinition names
 *                4) Loads each referenced PrimaryDataAsset in turn
 *                5) Broadcasts per‐asset loaded events and final success/failure events
 * -------------------------------------------------------------------------------
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Enums/AssetDefinitionType.h"
#include "Structs/AssetLoaderEvents.h"
#include "AssetManagedLoaderHelper.generated.h"

/**
 * UAssetManagedLoaderHelper
 *
 * A completely static helper class that orchestrates:
 *   • Async loading of a single “definition” PrimaryDataAsset by FName
 *   • Invoking either HandleDefinitionAssetLoaded or HandleDefinitionAssetLoadFailed
 *   • In HandleDefinitionAssetLoaded:
 *       – Matching a supplied UClass against the asset’s selectors
 *       – Building FName paths (DA_<AssetType>_<DefiningName>) for each AssetDefinition
 *       – Broadcasting FOnDefinitionAssetsPreLoad with those paths
 *       – Invoking LoadPrimaryAssetsByList to load each referenced PrimaryDataAsset
 *   • LoadPrimaryAssetsByList / LoadPrimaryAssetsByName:
 *       – Constructing each PrimaryAssetId from AssetType + DefiningAssetName
 *       – Async‐loading each asset and calling PrimaryAssetLoaded
 *   • PrimaryAssetLoaded:
 *       – Mapping back to the original index
 *       – Broadcasting FOnPrimaryAssetLoaded for each loaded object
 */
UCLASS()
class ASSETLOADER_API UAssetManagedLoaderHelper : public UObject
{
    GENERATED_BODY()

public:
    /**
     * Starts the workflow:
     * 1) Async‐load the Definition DataAsset (e.g. "DA_<Type>_<Name>")
     * 2) On success, HandleDefinitionAssetLoaded is called
     * 3) On failure, HandleDefinitionAssetLoadFailed is called
     *
     * @param Owner                Context object for callbacks
     * @param DefinitionAssetName  Name of the PrimaryDataAsset to load
     */
    UFUNCTION(BlueprintCallable, Category = "AssetWorkflow|Static")
    static int32 LoadDefinitionConfigAssets(
        const FAssetLoaderEvents& Events,
        EAssetDefinitionType      AssetType,
        UObject* Owner,
        bool bComponentHasComponentAssetToLoad = true,
        bool bComponentHasActorAssetToLoad = true,
		bool bDebugRoute = false
    );

    /**
     * Loads all provided soft objects (UObject) and soft classes (UClass) sequentially.
     * @param WorldContextObject Any UObject, e.g., an Actor or GameInstance. Serves as the Outer for the helper.
     * @param ObjectPtrs         Array of TSoftObjectPtr<UObject> (soft object references). Invalid entries are skipped.
     * @param ClassPtrs          Array of TSoftClassPtr<UObject> (soft class references). Invalid entries are skipped.
     * @return                   An instance of UAsyncAssetLoaderHelper*, to which you can bind the OnAllAssetsLoaded event.
     */
    UFUNCTION(BlueprintCallable, Category = "Async Loading", meta = (WorldContext = "WorldContextObject"))
    static class UAsyncAssetLoaderHelper* LoadObjectsAndClassesAsync(
        UObject* WorldContextObject,
        const TArray<TSoftObjectPtr<UObject>>& ObjectPtrs,
        const TArray<TSoftClassPtr<UObject>>& ClassPtrs
    );

    UFUNCTION(BlueprintCallable, Category = "Async Loading", meta = (WorldContext = "WorldContextObject"))
    static class UControllerAsyncAssetLoaderHelper* LoadInputAssetsAsync(
        UObject* WorldContextObject,
        const TArray<TSoftObjectPtr<UObject>>& ObjectPtrs,
        const TArray<TSoftClassPtr<UObject>>& ClassPtrs,
		const uint8& InputConfigType
    );

private:

    /** Handles loading of input assets (e.g. InputConfigPrimaryDataAssetSet) */
    static int32 HandleInputAssetLoading(const FAssetLoaderEvents& Events, UObject* Owner, UObject* ActiveOwner = nullptr);

    /** Called when the input assets are loaded */
    static void OnInputAssetsLoaded(const FAssetLoaderEvents& Events, UObject* Owner, TArray<FSoftObjectPath> AssetPaths);

    /** Helper: handles loading of PrimaryAssets by FName paths */
    static int32 HandlePrimaryAssetLoading(const FAssetLoaderEvents& Events, UObject* Owner, UObject* ActiveOwner = nullptr, bool bDebugRoute = false);

    /** Called when the Definition DataAsset loads successfully */
    static void HandlePrimaryDefinitionAssetLoaded(const FAssetLoaderEvents& Events, UObject* Owner, class UPrimaryDataAssetSet* LoadedDefAsset, bool bDebugRoute = false);

    /** Called when pre‐loading assets are loaded */
    static void HandlePreLoadingAssetsLoaded(const FAssetLoaderEvents& Events, class UObject* Owner, TArray<FSoftObjectPath> AssetPaths, bool bDebugRoute = false);

    /** Called when the Definition DataAsset fails to load */
    UFUNCTION(BlueprintCallable, Category = "Async Loading", meta = (WorldContext = "WorldContextObject"))
    static void LoadPreLoadingAssetAsync(
        const FAssetLoaderEvents& Events,
        UObject* Owner,
        class UPreLoadingDataAsset* PreloadinAsset,
		bool bDebugRoute = false
    );

    /** Called when the Definition DataAsset loads successfully */
    static void HandleDefinitionAssetLoaded(
        const FAssetLoaderEvents& Events,
        UObject* Owner,
        class UPreLoadingDataAsset* LoadedDefAsset,
		bool bDebugRoute = false
    );

    /** Called when the Definition DataAsset fails to load */
    static void HandleDefinitionAssetLoadFailed(const FAssetLoaderEvents& Events, UObject* Owner, bool HasClassesToLoad = true);
	
	/** Helper function to retrieve soft object paths from the given asset */
	static TArray<FSoftObjectPath> GetSoftObjectPathsFromAsset(class UPreLoadingDataAsset* PreLoadingAsset);
};

//Aber ich würde dir trotzdem empfehlen, die Klasse in naher Zukunft zu refaktorisieren, um…
//
//Einheitliche Delegate - Bindings
//Verwende überall die gleiche Methode(z.B.CreateLambda) anstatt zwischen CreateStatic, CreateLambda und CreateUObject zu springen.Das macht klar, wie State in die Callback - Lambdas kommt und vermeidet Verwirrung.
//
//Einen einzigen Async - API - Stil
//Entweder durchgehend UAssetManager::LoadPrimaryAsset(...) oder nur StreamableManager::RequestAsyncLoad(...).So weiß jeder Leser sofort, wie das Nachladen funktioniert.
//
//Konsistente Parameterübergabe
//Listen per const TArray<FName>& und einzelne Werte per const FName& übergeben, oder wenn du unbedingt kopieren willst, dann klar dokumentiert per TSharedPtr<TArray<FName>>.Das vermeidet versehentliche Kopien oder Dangling - References.
//
//Zentralisierte Fehler - Events
//Stelle sicher, dass du OnPrimaryAssetClassNotFound und OnPrimaryAssetClassMatchFound immer genau an den richtigen Stellen feuerst – idealerweise in einer kleinen Hilfsfunktion, damit du beim Refactoring nur einen Ort anpassen musst.