#pragma once

#include "CoreMinimal.h"
#include "AssetLoaderEvents.generated.h"

/**
 * Fired when one of the AssetDefinitions has finished loading.
 * @param LoadedObject  The UObject instance loaded
 * @param AssetIndex    The index into the original AssetPaths array
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnPrimaryAssetLoaded,
    UObject*, Owner,
    UObject*, LoadedObject
);

/**
 * Fired when the Definition PrimaryAsset could not be loaded.
 * @param Owner  The context object that requested the load
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnPrimaryAssetClassNotFound,
    UObject*, Owner,
    bool, HasClassesToLoad = true
);

USTRUCT(BlueprintType)
struct FAssetLoaderEvents
{
    GENERATED_BODY()

public:
    /** Delegate when each AssetDefinition object is loaded */
    FOnPrimaryAssetLoaded            OnPrimaryAssetLoaded;

    /** Delegate when the Definition PrimaryAsset could not be loaded */
    FOnPrimaryAssetClassNotFound     OnPrimaryAssetClassNotFound;

};