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

#include "DataAssets/Primary/InputConfigPrimaryDataAssetSet.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

#include "Interfaces/AssetDefinitionInformationInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(Log_AssetManagedLoaderHelper_InputAssets, Log, All);

int32 UAssetManagedLoaderHelper::HandleInputAssetLoading(const FAssetLoaderEvents& Events, UObject* Owner, UObject* ActiveOwner)
{
    UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Verbose,
        "{Time}: {Line} {Class}: HandleInputAssetLoading() called. Owner={Owner}",
        ("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME),
        ("Owner", Owner ? *GetNameSafe(Owner) : TEXT("nullptr"))
    );

    if (!Owner)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Error,
            "{Time}: {Line} {Class}: Invalid Owner (nullptr).",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, nullptr);
        return 0;
    }

    TArray<FPrimaryAssetId> AssetIds;
    UAssetManager& AssetManager = UAssetManager::Get();
    AssetManager.GetPrimaryAssetIdList(UInputConfigPrimaryDataAssetSet::GetAssetType(), AssetIds);

    TArray<FSoftObjectPath> AssetPaths;
    for (const FPrimaryAssetId& Id : AssetIds)
    {
        FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(Id);
        if (AssetPath.IsValid())
        {
            AssetPaths.Add(AssetPath);
        }
    }

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    Streamable.RequestAsyncLoad(
        AssetPaths,
        FStreamableDelegate::CreateLambda([Events, Owner, AssetPaths]()
            {
                UAssetManagedLoaderHelper::OnInputAssetsLoaded(Events, Owner, AssetPaths);
            })
    );

    UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Verbose,
        "{Time}: {Line} {Class}: RequestAsyncLoad for InputConfigDataAssetCollection with {NumAssets} assets.",
        ("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME),
        ("NumAssets", AssetPaths.Num())
	);

	return AssetPaths.Num();
}

void UAssetManagedLoaderHelper::OnInputAssetsLoaded(const FAssetLoaderEvents& Events, UObject* Owner, TArray<FSoftObjectPath> AssetPaths)
{
        UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Verbose,
        "{Time}: {Line} {Class}: OnInputAssetsLoaded() called. Owner={Owner}, NumAssets={NumAssets}",
        ("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME),
        ("Owner", Owner ? *GetNameSafe(Owner) : TEXT("nullptr")),
        ("NumAssets", AssetPaths.Num())
    );

    if (!Owner)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Error,
            "{Time}: {Line} {Class}: Invalid Owner (nullptr).",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, nullptr);
        return;
    }
    for (int32 i = 0; i < AssetPaths.Num(); i++)
    {
		const FSoftObjectPath& Path = AssetPaths[i];

        UObject* LoadedObj = Path.ResolveObject();
        if (!LoadedObj)
            LoadedObj = Path.TryLoad();

        if (auto* Asset = Cast<UInputConfigPrimaryDataAssetSet>(LoadedObj))
        {
            if (Events.OnPrimaryAssetLoaded.IsBound())
            {
                Events.OnPrimaryAssetLoaded.Broadcast(Owner, LoadedObj);
                UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Verbose,
                    "{Time}: {Line} {Class}: InputConfigDataAssetCollection loaded successfully: {Path}",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
                    ("Line", GET_LINE_NUMBER),
                    ("Time", GET_CURRENT_TIME),
                    ("Path", *Path.ToString())
                );
            }
            else
            {
                UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Warning,
					"{Time}: {Line} {Class}: OnPrimaryAssetLoaded delegate is not bound. Asset loaded: {Path}",
                    ("Class", GET_CLASSNAME_WITH_FUNCTION),
                    ("Line", GET_LINE_NUMBER),
                    ("Time", GET_CURRENT_TIME),
                    ("Path", *Path.ToString())
				);

                HandleDefinitionAssetLoadFailed(
                    Events,
                    Owner,
                    true
                );
            }
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Error,
                "{Time}: {Line} {Class}: Loaded object is not a valid UInputConfigDataAssetCollection: {Path}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Path", *Path.ToString())
			);

            HandleDefinitionAssetLoadFailed(
				Events,
                Owner,
                true
			);

			continue;
        }
    }

    UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Verbose,
        "{Time}: {Line} {Class}: All InputConfigDataAssets loaded successfully.",
        ("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME)
	);
}

UControllerAsyncAssetLoaderHelper* UAssetManagedLoaderHelper::LoadInputAssetsAsync(UObject* WorldContextObject, const TArray<TSoftObjectPtr<UObject>>& ObjectPtrs, const TArray<TSoftClassPtr<UObject>>& ClassPtrs, const uint8& InputConfigType)
{
    if (!WorldContextObject)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Warning,
            "{Time}: {Line} {Class}: LoadObjectsAndClassesAsync called with null WorldContextObject.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return nullptr;
    }

    // --- Create the loader instance ---
    UControllerAsyncAssetLoaderHelper* Loader = NewObject<UControllerAsyncAssetLoaderHelper>(WorldContextObject);
    if (!Loader)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Error,
            "{Time}: {Line} {Class}: Failed to create UAsyncAssetLoaderHelper instance.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return nullptr;
    }

    Loader->AddToRoot();

    UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Verbose,
        "{Time}: {Line} {Class}: Created UAsyncAssetLoaderHelper instance. Preparing to gather paths...",
        ("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME)
    );

    // --- Gather valid paths from object & class pointers ---
    TArray<FSoftObjectPath> PathsToLoad;
    PathsToLoad.Reserve(ObjectPtrs.Num() + ClassPtrs.Num());

    int32 SkippedObjectPaths = 0;
    int32 SkippedClassPaths = 0;

    for (const TSoftObjectPtr<UObject>& SoftObj : ObjectPtrs)
    {
        const FSoftObjectPath Path = SoftObj.ToSoftObjectPath();
        if (Path.IsValid())
        {
            PathsToLoad.Add(Path);
            UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, VeryVerbose,
                "{Time}: {Line} {Class}: Added ObjectPath to load list: {Path}",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Path", *Path.ToString())
            );
        }
        else
        {
            ++SkippedObjectPaths;
        }
    }

    for (const TSoftClassPtr<UObject>& SoftClass : ClassPtrs)
    {
        const FSoftObjectPath Path = SoftClass.ToSoftObjectPath();
        if (Path.IsValid())
        {
            PathsToLoad.Add(Path);
            UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, VeryVerbose,
                "{Time}: {Line} {Class}: Added ClassPath to load list: {Path}",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Path", *Path.ToString())
            );
        }
        else
        {
            ++SkippedClassPaths;
        }
    }

    UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Verbose,
        "{Time}: {Line} {Class}: Async load request prepared with {ValidCount} total paths "
        "({ObjectCount} objects, {ClassCount} classes). Skipped: {SkippedObj} object(s), {SkippedCls} class(es).",
        ("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME),
        ("ValidCount", PathsToLoad.Num()),
        ("ObjectCount", ObjectPtrs.Num()),
        ("ClassCount", ClassPtrs.Num()),
        ("SkippedObj", SkippedObjectPaths),
        ("SkippedCls", SkippedClassPaths)
    );

    // --- Start loading ---
    Loader->StartLoading(PathsToLoad, InputConfigType);

    UE_LOGFMT(Log_AssetManagedLoaderHelper_InputAssets, Verbose,
        "{Time}: {Line} {Class}: UAsyncAssetLoaderHelper::StartLoading called with {Count} path(s).",
        ("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME),
        ("Count", PathsToLoad.Num())
    );

    return Loader;
}