// AssetManagedLoaderHelper.cpp
/*
 * Copyright (c) 2024 Yves Tanas
 * All Rights Reserved.
 *
 * Implements UAssetManagedLoaderHelper, a static helper that:
 *   1) Asynchronously loads a “definition” PrimaryDataAsset by FName
 *   2) Matches a supplied UClass against its selectors
 *   3) Broadcasts a pre‐load event with the list of derived AssetDefinition names
 *   4) Loads each referenced PrimaryDataAsset in turn
 *   5) Broadcasts per‐asset loaded events and final success/failure events
 *
 * All methods are static and use delegates to carry state.
 */

#include "AssetManager/AssetManagedLoaderHelper.h"

#include "AssetManager/ControllerAsyncAssetLoaderHelper.h"

#include "DataAssets/Primary/PrimaryDataAssetSet.h"
#include "DataAssets/PreLoadingDataAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

#include "Interfaces/AssetDefinitionInformationInterface.h"
#include "Interfaces/PreLoadingAssetInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(Log_AssetManagedLoaderHelper_AssetLoading, Log, All);

int32 UAssetManagedLoaderHelper::HandlePrimaryAssetLoading(const FAssetLoaderEvents& Events, UObject* Owner, UObject* ActiveOwner, bool bDebugRoute)
{
    if(bDebugRoute)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
			"{Time}: {Line} {Class}: HandleAssetLoading called with Owner: {Owner}, ActiveOwner: {ActiveOwner}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", Owner ? *Owner->GetName() : TEXT("nullptr")),
            ("ActiveOwner", ActiveOwner ? *ActiveOwner->GetName() : TEXT("nullptr")));
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
            "{Time}: {Line} {Class}: HandleAssetLoading called with Owner: {Owner}, ActiveOwner: {ActiveOwner}.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Owner", Owner ? *Owner->GetName() : TEXT("nullptr")),
            ("ActiveOwner", ActiveOwner ? *ActiveOwner->GetName() : TEXT("nullptr")));
    }

    if (!Owner)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Error,
            "{Time}: {Line} {Class}: Invalid Owner (nullptr).",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, nullptr);
        return 0;
    }

    if (!Owner->GetClass()->ImplementsInterface(UAssetDefinitionInformationInterface::StaticClass()))
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Error,
            "{Time}: {Line} {Class}: Owner {Owner} does not implement UAssetDefinitionInformationInterface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Owner", *Owner->GetName()));
        HandleDefinitionAssetLoadFailed(Events, nullptr);
		return 0;
    }

	FName AssetDefinitionName = IAssetDefinitionInformationInterface::Execute_GetDefinitionAssetName(Owner);

    if (AssetDefinitionName.IsEqual(FName("BaseActorDefinition")))
    {
        if (bDebugRoute)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
                "{Time}: {Line} {Class}: Owner {Owner} has an empty AssetDefinitionName.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Owner", *Owner->GetName()));
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
                "{Time}: {Line} {Class}: Owner {Owner} has an empty AssetDefinitionName.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Owner", *Owner->GetName()));
        }
        return 0;
    }

    if(bDebugRoute)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
            "{Time}: {Line} {Class}: Owner {Owner} has AssetDefinitionName: {AssetDefinitionName}.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("AssetDefinitionName", *AssetDefinitionName.ToString()),
            ("Owner", *Owner->GetName()));
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
            "{Time}: {Line} {Class}: Owner {Owner} has AssetDefinitionName: {AssetDefinitionName}.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("AssetDefinitionName", *AssetDefinitionName.ToString()),
            ("Owner", *Owner->GetName()));
    }

    if (AssetDefinitionName.IsNone())
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Error,
			"{Time}: {Line} {Class}: Owner {Owner} has no valid AssetDefinitionName.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Owner", *Owner->GetName()));
        HandleDefinitionAssetLoadFailed(Events, nullptr);
        return 0;
	}
    else
    {
        if (bDebugRoute)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
                "{Time}: {Line} {Class}: Owner {Owner} has valid AssetDefinitionName: {AssetDefinitionName}.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Owner", *Owner->GetName()),
                ("AssetDefinitionName", *AssetDefinitionName.ToString()));
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
                "{Time}: {Line} {Class}: Owner {Owner} has valid AssetDefinitionName: {AssetDefinitionName}.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Owner", *Owner->GetName()),
                ("AssetDefinitionName", *AssetDefinitionName.ToString()));
        }
    }

    FString SetAssetNameAsString = "DA_" + UPrimaryDataAssetSet::GetAssetType().ToString() + "_" + AssetDefinitionName.ToString();
    FName AssetNameFName(*SetAssetNameAsString);
    FPrimaryAssetId AssetId(UPrimaryDataAssetSet::GetAssetType(), AssetNameFName);

    if(bDebugRoute)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
            "{Time}: {Line} {Class}: Requesting async load for DefinitionAssetId = {AssetId}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("AssetId", *AssetId.ToString())
        );
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
            "{Time}: {Line} {Class}: Requesting async load for DefinitionAssetId = {AssetId}",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("AssetId", *AssetId.ToString())
        );
	}

    // Prepare weak pointer for callback
    TStrongObjectPtr<UObject> OwnerPtr(ActiveOwner ? ActiveOwner : Owner);
    FName OwnerName = ActiveOwner ? ActiveOwner->GetFName() : Owner ? Owner->GetFName() : NAME_None;

    // Create the streamable delegate
    FStreamableDelegate OnLoaded = FStreamableDelegate::CreateLambda(
        [OwnerPtr, AssetId, OwnerName, Events, bDebugRoute]()
        {
            UObject* RealOwner = OwnerPtr.Get();
            if (!RealOwner)
            {
                UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Warning,
                    "{Time}: {Line} {Class}: Owner expired before asset loaded: {Owner}",
                    ("Class", GET_CLASSNAME_WITH_FUNCTION),
                    ("Line", GET_LINE_NUMBER),
                    ("Time", GET_CURRENT_TIME),
                    ("Owner", OwnerName));
                HandleDefinitionAssetLoadFailed(Events, nullptr);
                return;
            }

            // Retrieve and cast the loaded object
            UObject* LoadedObj = UAssetManager::Get().GetPrimaryAssetObject(AssetId);
            auto* DefAsset = Cast<UPrimaryDataAssetSet>(LoadedObj);

            if (DefAsset)
            {
                if(bDebugRoute)
                {
                    UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
                        "{Time}: {Line} {Class}: Definition DataAsset loaded: {AssetId}",
						("Class", GET_CLASSNAME_WITH_FUNCTION),
                        ("Line", GET_LINE_NUMBER),
                        ("Time", GET_CURRENT_TIME),
                        ("AssetId", *AssetId.ToString())
                    );
                }
                else
                {
                    UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, VeryVerbose,
                        "{Time}: {Line} {Class}: Definition DataAsset loaded: {AssetId}",
                        ("Class", GET_CLASSNAME_WITH_FUNCTION),
                        ("Line", GET_LINE_NUMBER),
                        ("Time", GET_CURRENT_TIME),
                        ("AssetId", *AssetId.ToString())
                    );
                }

                HandlePrimaryDefinitionAssetLoaded(Events, RealOwner, DefAsset, bDebugRoute);
            }
            else
            {
                UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Error,
                    "{Time}: {Line} {Class}: Failed to load Definition DataAsset: {AssetId} on Owner {Owner} with RealOwner {RealOwner}",
                    ("Class", GET_CLASSNAME_WITH_FUNCTION),
                    ("Line", GET_LINE_NUMBER),
                    ("Time", GET_CURRENT_TIME),
                    ("AssetId", *AssetId.ToString()),
                    ("Owner", *OwnerName.ToString()),
                    ("RealOwner", *RealOwner->GetName())
                );
                HandleDefinitionAssetLoadFailed(Events, RealOwner);
            }
        }
    );

    // Fire off the async load
    UAssetManager::Get().LoadPrimaryAsset(
        AssetId,
        TArray<FName>() /* no bundles */,
        OnLoaded
    );

	// Optionally, you can also unload the asset if needed
    //UAssetManager::Get().UnloadPrimaryAsset(
    //    AssetId
    //);

    // NEW LOG
    UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
        "{Time}: {Line} {Class}: LoadPrimaryAsset request queued for DefinitionAssetId = {AssetId}",
        ("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME),
        ("AssetId", *AssetId.ToString())
    );

    return 0;
}

void UAssetManagedLoaderHelper::HandlePrimaryDefinitionAssetLoaded(const FAssetLoaderEvents& Events, UObject* Owner, UPrimaryDataAssetSet* LoadedDefAsset, bool bDebugRoute)
{
    if (!Owner)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Error,
            "{Time}: {Line} {Class}: Invalid Owner (nullptr) in HandlePrimaryDefinitionAssetLoaded.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, nullptr);
        return;
    }
    else
    {
        if (bDebugRoute)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
                "{Time}: {Line} {Class}: HandlePrimaryDefinitionAssetLoaded called with Owner: {Owner}.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Owner", *Owner->GetName()));
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
                "{Time}: {Line} {Class}: HandlePrimaryDefinitionAssetLoaded called with Owner: {Owner}.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Owner", *Owner->GetName()));
        }
    }

    if (!LoadedDefAsset)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Error,
            "{Time}: {Line} {Class}: LoadedDefAsset is nullptr in HandlePrimaryDefinitionAssetLoaded.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, Owner);
        return;
    }

    UPrimaryDataAssetSet* PrimaryDataAssetSet = Cast<UPrimaryDataAssetSet>(LoadedDefAsset);

    if (!PrimaryDataAssetSet)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Error,
            "{Time}: {Line} {Class}: LoadedDefAsset is not a valid UPrimaryDataAssetSet.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, Owner);
        return;
    }
    else {
        if (bDebugRoute)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display, "{Time}: {Line} {Class}: LoadedDefAsset is a valid UPrimaryDataAssetSet.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME));
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose, "{Time}: {Line} {Class}: LoadedDefAsset is a valid UPrimaryDataAssetSet.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME));
        }
    }

    // Broadcast the pre-load event with the list of derived AssetDefinition names
    TArray<TSoftObjectPtr<UPreLoadingDataAsset>> DerivedAsses = LoadedDefAsset->AssetSet;

    if (DerivedAsses.Num() == 0)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Warning,
            "{Time}: {Line} {Class}: No derived assets found in LoadedDefAsset: {LoadedDefAsset}.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("LoadedDefAsset", *LoadedDefAsset->GetName()));
        HandleDefinitionAssetLoadFailed(Events, Owner);
        return;
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, VeryVerbose,
            "{Time}: {Line} {Class}: {Num} derived assets found in LoadedDefAsset: {LoadedDefAsset}.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Num", DerivedAsses.Num()),
            ("LoadedDefAsset", *LoadedDefAsset->GetName()));
    }

    TArray<FSoftObjectPath> AssetPaths;
    for (const TSoftObjectPtr<UPreLoadingDataAsset>& SoftObj : DerivedAsses)
    {
        if (!SoftObj.IsNull())
        {
            AssetPaths.Add(SoftObj.ToSoftObjectPath());
        }
    }

    if (AssetPaths.Num() == 0)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Warning,
            "{Time}: {Line} {Class}: No valid asset paths found in DerivedAsses.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, Owner);
        return;
    }
    else
    {
        if (bDebugRoute)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
                "{Time}: {Line} {Class}: {Num} asset paths found for LoadedDefAsset: {LoadedDefAsset}.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Num", AssetPaths.Num()),
                ("LoadedDefAsset", *LoadedDefAsset->GetName()));
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, VeryVerbose,
                "{Time}: {Line} {Class}: {Num} asset paths found for LoadedDefAsset: {LoadedDefAsset}.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Num", AssetPaths.Num()),
                ("LoadedDefAsset", *LoadedDefAsset->GetName()));
        }
    }

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    Streamable.RequestAsyncLoad(
        AssetPaths,
        FStreamableDelegate::CreateLambda([Events, Owner, bDebugRoute, AssetPaths]()
            {
                UAssetManagedLoaderHelper::HandlePreLoadingAssetsLoaded(Events, Owner, AssetPaths, bDebugRoute);
            })
    );

    if (bDebugRoute)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
            "{Time}: {Line} {Class}: Requesting async load for PreLoadingAssets: {AssetPaths}",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("AssetPaths", AssetPaths.Num() > 0 ? *AssetPaths[0].ToString() : TEXT("No Paths"))
        );
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
            "{Time}: {Line} {Class}: Requesting async load for PreLoadingAssets: {AssetPaths}",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("AssetPaths", AssetPaths.Num() > 0 ? *AssetPaths[0].ToString() : TEXT("No Paths"))
        );
    }
}

void UAssetManagedLoaderHelper::HandlePreLoadingAssetsLoaded(const FAssetLoaderEvents& Events, UObject* Owner, TArray<FSoftObjectPath> AssetPaths, bool bDebugRoute)
{
    if (!Owner)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Error,
            "{Time}: {Line} {Class}: Invalid Owner (nullptr) in HandlePrimaryDefinitionAssetLoaded.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, nullptr);
        return;
    }

    if(AssetPaths.Num() == 0)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Warning,
            "{Time}: {Line} {Class}: No asset paths provided in HandlePreLoadingAssetsLoaded.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, Owner);
        return;
	}

    if(bDebugRoute)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
			"{Time}: {Line} {Class}: Handling pre-loading assets for Owner: {Owner}, AssetPaths: {AssetPaths}",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Owner", Owner ? *Owner->GetName() : TEXT("nullptr")),
            ("AssetPaths", AssetPaths.Num() > 0 ? *AssetPaths[0].ToString() : TEXT("No Paths"))
        );
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
            "{Time}: {Line} {Class}: Handling pre-loading assets for Owner: {Owner}, AssetPaths: {AssetPaths}",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Owner", Owner ? *Owner->GetName() : TEXT("nullptr")),
            ("AssetPaths", AssetPaths.Num() > 0 ? *AssetPaths[0].ToString() : TEXT("No Paths"))
        );
    }

    TArray<UPreLoadingDataAsset*> LoadedAssets;
    for (const FSoftObjectPath& Path : AssetPaths)
    {
        UObject* LoadedObj = Path.ResolveObject();
        if (!LoadedObj)
        {
			LoadedObj = Path.TryLoad(); // Attempt to load the object if it wasn't already loaded
        }
        if (auto* PreloadAsset = Cast<UPreLoadingDataAsset>(LoadedObj))
        {
            LoadedAssets.Add(PreloadAsset);
        }
    }

    if(bDebugRoute)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
            "{Time}: {Line} {Class}: Loaded {Num} PreLoadingDataAssets from AssetPaths.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
            ("Num", LoadedAssets.Num()));
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, VeryVerbose,
            "{Time}: {Line} {Class}: Loaded {Num} PreLoadingDataAssets from AssetPaths.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Num", LoadedAssets.Num()));
    }

    if(LoadedAssets.Num() == 0)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Warning,
            "{Time}: {Line} {Class}: No valid UPreLoadingDataAssets loaded from AssetPaths.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, Owner);
		return;
	}
    else
    {
        if (bDebugRoute)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
				"{Time}: {Line} {Class}: Successfully loaded {Num} UPreLoadingDataAssets.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
                ("Num", LoadedAssets.Num()));
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, VeryVerbose,
                "{Time}: {Line} {Class}: Successfully loaded {Num} UPreLoadingDataAssets.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Num", LoadedAssets.Num()));
        }
    }

    for (auto* PreloadAsset : LoadedAssets)
    {
        if (!PreloadAsset)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Warning,
                "{Time}: {Line} {Class}: Found nullptr in LoadedAssets.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME));
            continue;
        }

        if(bDebugRoute)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
                "{Time}: {Line} {Class}: Loading PreLoadingAsset: {PreloadAsset}",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
                ("PreloadAsset", *PreloadAsset->GetName()));
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
                "{Time}: {Line} {Class}: Loading PreLoadingAsset: {PreloadAsset}",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("PreloadAsset", *PreloadAsset->GetName()));
        }

	    LoadPreLoadingAssetAsync(Events, Owner, PreloadAsset, bDebugRoute);
    }

    if(bDebugRoute)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
			"{Time}: {Line} {Class}: Finished handling pre-loading assets for Owner: {Owner}.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Owner", Owner ? *Owner->GetName() : TEXT("nullptr"))
        );
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
            "{Time}: {Line} {Class}: Finished handling pre-loading assets for Owner: {Owner}.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Owner", Owner ? *Owner->GetName() : TEXT("nullptr"))
        );
    }
}

void UAssetManagedLoaderHelper::LoadPreLoadingAssetAsync(const FAssetLoaderEvents& Events, UObject* Owner, UPreLoadingDataAsset* PreloadinAsset, bool bDebugRoute)
{
    if (!Owner)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Error,
            "{Time}: {Line} {Class}: Invalid Owner (nullptr) in LoadPreLoadingAssetAsync.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, nullptr);
        return;
    }

    if (!PreloadinAsset)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Error,
            "{Time}: {Line} {Class}: PreloadingAsset is nullptr in LoadPreLoadingAssetAsync.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, Owner);
        return;
	}

	TArray<FSoftObjectPath> AssetPaths = GetSoftObjectPathsFromAsset(PreloadinAsset);

    if (AssetPaths.Num() == 0)
    {
        if(bDebugRoute)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
                "{Time}: {Line} {Class}: PreloadinAsset {PreloadinAsset} has no asset classes or objects to load.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("PreloadinAsset", *PreloadinAsset->GetName()));
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
                "{Time}: {Line} {Class}: PreloadinAsset {PreloadinAsset} has no asset classes or objects to load.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("PreloadinAsset", *PreloadinAsset->GetName()));
        }

		HandleDefinitionAssetLoaded(Events, Owner, PreloadinAsset, bDebugRoute);
        return;
    }
    else
    {
        if (bDebugRoute)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
				"{Time}: {Line} {Class}: PreloadinAsset {PreloadinAsset} has {Num} asset classes or objects to load.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("PreloadinAsset", *PreloadinAsset->GetName()),
            ("Num", AssetPaths.Num()));
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, VeryVerbose,
                "{Time}: {Line} {Class}: PreloadinAsset {PreloadinAsset} has {Num} asset classes or objects to load.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("PreloadinAsset", *PreloadinAsset->GetName()),
                ("Num", AssetPaths.Num()));
        }
    }

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    Streamable.RequestAsyncLoad(
        AssetPaths,
        FStreamableDelegate::CreateLambda([Events, Owner, PreloadinAsset, bDebugRoute]()
            {
                UAssetManagedLoaderHelper::HandleDefinitionAssetLoaded(Events, Owner, PreloadinAsset, bDebugRoute);
            })
    );

    if (bDebugRoute)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
            "{Time}: {Line} {Class}: Requesting async load for PreLoadingAsset: {PreloadinAsset}",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("PreloadinAsset", *PreloadinAsset->GetName())
        );
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
            "{Time}: {Line} {Class}: Requesting async load for PreLoadingAsset: {PreloadinAsset}",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("PreloadinAsset", *PreloadinAsset->GetName())
        );
    }
}

void UAssetManagedLoaderHelper::HandleDefinitionAssetLoaded(const FAssetLoaderEvents& Events, UObject* Owner, UPreLoadingDataAsset* LoadedDefAsset, bool bDebugRoute)
{
    if (!Owner)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Error,
            "{Time}: {Line} {Class}: Invalid Owner (nullptr) in LoadPreLoadingAssetAsync.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, nullptr);
        return;
    }

    if (!LoadedDefAsset)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Error,
            "{Time}: {Line} {Class}: LoadedDefAsset is nullptr in HandleDefinitionAssetLoaded.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, Owner);
        return;
    }

    if(bDebugRoute)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Display,
			"{Time}: {Line} {Class}: Definition DataAsset loaded: {LoadedDefAsset}",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("LoadedDefAsset", *LoadedDefAsset->GetName())
        );
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_AssetLoading, Verbose,
            "{Time}: {Line} {Class}: Definition DataAsset loaded: {LoadedDefAsset}",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("LoadedDefAsset", *LoadedDefAsset->GetName())
        );
    }

    Events.OnPrimaryAssetLoaded.Broadcast(Owner, LoadedDefAsset);
    // Optionally, you can also unload the asset if needed
    //UAssetManager::Get().UnloadPrimaryAsset(
    //    LoadedDefAsset->GetPrimaryAssetId()
	//);
}
