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
#include "AssetManager/AsyncAssetLoaderHelper.h"

#include "DataAssets/PreLoadingDataAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

#include "Interfaces/AssetDefinitionInformationInterface.h"
#include "Interfaces/PreLoadingAssetInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(Log_AssetManagedLoaderHelper, Log, All);

int32 UAssetManagedLoaderHelper::LoadDefinitionConfigAssets(const FAssetLoaderEvents& Events, EAssetDefinitionType AssetType, UObject* Owner, bool bComponentHasComponentAssetToLoad, bool bComponentHasActorAssetToLoad, bool bDebugRoute)
{
    if (bDebugRoute)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, Display,
            "{Time}: {Line} {Class}: LoadDefinitionConfigAssets() called. Owner={Owner}, AssetType={AssetType}",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Owner", Owner ? *GetNameSafe(Owner) : TEXT("nullptr")),
            ("AssetType", *UEnum::GetValueAsString(AssetType))
        );
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, Verbose,
            "{Time}: {Line} {Class}: LoadDefinitionConfigAssets() called. Owner={Owner}, AssetType={AssetType}",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Owner", Owner ? *GetNameSafe(Owner) : TEXT("nullptr")),
            ("AssetType", *UEnum::GetValueAsString(AssetType))
        );
    }


    if (!Owner)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, Error,
            "{Time}: {Line} {Class}: Invalid Owner (nullptr).",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, nullptr);
        return 0;
    }

    if(AssetType == EAssetDefinitionType::ADT_NONE)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, Error,
            "{Time}: {Line} {Class}: Invalid AssetType (None).",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, Owner);
        return 0;
    }
    else if(AssetType == EAssetDefinitionType::ADT_INPUT)
    {
        if(bDebugRoute)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper, Display,
                "{Time}: {Line} {Class}: ADT_INPUT signal. Is loading InputConfigAssets.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME));
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper, Verbose,
                "{Time}: {Line} {Class}: ADT_INPUT signal. Is loading InputConfigAssets.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME));
        }

		return HandleInputAssetLoading(Events, Owner);
    }

    if (!Owner->GetClass()->ImplementsInterface(UAssetDefinitionInformationInterface::StaticClass()))
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, Error,
            "{Time}: {Line} {Class}: Owner does not implement IAssetDefinitionInformationInterface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        HandleDefinitionAssetLoadFailed(Events, Owner);
		return 0;
    }

	FName OwnerDefinitionAssetName = IAssetDefinitionInformationInterface::Execute_GetDefinitionAssetName(Owner);

    if (!(AssetType == EAssetDefinitionType::ADT_COMPONENT && !bComponentHasComponentAssetToLoad))
    {
        if(bDebugRoute)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper, Display,
                "{Time}: {Line} {Class}: ADT_ACTOR signal. Owner={Owner}, OwnerDefinitionAssetName={OwnerDefinitionAssetName}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Owner", Owner ? *GetNameSafe(Owner) : TEXT("nullptr")),
                ("OwnerDefinitionAssetName", *OwnerDefinitionAssetName.ToString())
            );
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper, Verbose,
                "{Time}: {Line} {Class}: ADT_ACTOR signal. Owner={Owner}, OwnerDefinitionAssetName={OwnerDefinitionAssetName}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Owner", Owner ? *GetNameSafe(Owner) : TEXT("nullptr")),
                ("OwnerDefinitionAssetName", *OwnerDefinitionAssetName.ToString())
            );
		}
        HandlePrimaryAssetLoading(Events, Owner, nullptr, bDebugRoute);
    }

    if (AssetType == EAssetDefinitionType::ADT_COMPONENT && bComponentHasActorAssetToLoad)
    {
        if (UActorComponent* Comp = Cast<UActorComponent>(Owner))
        {
            AActor* OwnerActor = Comp->GetOwner();

            if(bDebugRoute)
            {
                UE_LOGFMT(Log_AssetManagedLoaderHelper, Display,
                    "{Time}: {Line} {Class}: ADT_COMPONENT signal. OwnerActor={OwnerActor}, OwnerDefinitionAssetName={OwnerDefinitionAssetName}",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
                    ("Line", GET_LINE_NUMBER),
                    ("Time", GET_CURRENT_TIME),
                    ("OwnerActor", OwnerActor ? *GetNameSafe(OwnerActor) : TEXT("nullptr")),
                    ("OwnerDefinitionAssetName", *OwnerDefinitionAssetName.ToString())
                );
            }
            else
            {
                UE_LOGFMT(Log_AssetManagedLoaderHelper, Verbose,
                    "{Time}: {Line} {Class}: ADT_COMPONENT signal. OwnerActor={OwnerActor}, OwnerDefinitionAssetName={OwnerDefinitionAssetName}",
                    ("Class", GET_CLASSNAME_WITH_FUNCTION),
                    ("Line", GET_LINE_NUMBER),
                    ("Time", GET_CURRENT_TIME),
                    ("OwnerActor", OwnerActor ? *GetNameSafe(OwnerActor) : TEXT("nullptr")),
                    ("OwnerDefinitionAssetName", *OwnerDefinitionAssetName.ToString())
                );
            }

            if(!OwnerActor)
            {
                UE_LOGFMT(Log_AssetManagedLoaderHelper, Error,
                    "{Time}: {Line} {Class}: OwnerActor is nullptr for ADT_COMPONENT load.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
                    ("Line", GET_LINE_NUMBER),
                    ("Time", GET_CURRENT_TIME));
                HandleDefinitionAssetLoadFailed(Events, Owner);
                return 0;
            }

            if (!OwnerActor->GetClass()->ImplementsInterface(UAssetDefinitionInformationInterface::StaticClass()))
            {
                UE_LOGFMT(Log_AssetManagedLoaderHelper, Error,
					"{Time}: {Line} {Class}: OwnerActor {Actor} does not implement IAssetDefinitionInformationInterface.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
                    ("Line", GET_LINE_NUMBER),
                    ("Time", GET_CURRENT_TIME),
					("Actor", OwnerActor ? *GetNameSafe(OwnerActor) : TEXT("nullptr")));
                HandleDefinitionAssetLoadFailed(Events, Owner);
                return 0;
            }
			// Get the Definition Asset Name from the Owner Actor

	        OwnerDefinitionAssetName = IAssetDefinitionInformationInterface::Execute_GetDefinitionAssetName(OwnerActor);
		    // Start Actor Asset Loading Process
            HandlePrimaryAssetLoading(Events, OwnerActor, Owner, bDebugRoute);
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper, Error,
                "{Time}: {Line} {Class}: Owner is not a valid UActorComponent for ADT_COMPONENT load.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME));
            HandleDefinitionAssetLoadFailed(Events, Owner);
            return 0;
		}
    }

    return 0;
}

/**
 * @brief Handles failure of Definition asset loading.
 *
 * Broadcasts OnPrimaryAssetClassNotFound for the given Owner.
 *
 * @param Owner  Context object for which load failed.
 */
void UAssetManagedLoaderHelper::HandleDefinitionAssetLoadFailed(const FAssetLoaderEvents& Events, UObject* Owner, bool HasClassesToLoad)
{
    if (!Owner)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, Error,
            "{Time}: {Line} {Class}: HandleDefinitionAssetLoadFailed() called with nullptr Owner.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        Events.OnPrimaryAssetClassNotFound.Broadcast(nullptr, HasClassesToLoad);
        return;
    }

    UObject* Outer = Owner->GetOuter();
    if (Outer)
    {
        if (HasClassesToLoad)
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper, Verbose,
                "{Time}: {Line} {Class}: HandleDefinitionAssetLoadFailed() called for Owner={Owner} and Outer={Outer}",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Owner", Owner ? *GetNameSafe(Owner) : TEXT("nullptr")),
                ("Outer", Outer ? *GetNameSafe(Outer) : TEXT("nullptr"))
            );
        }
        else
        {
            UE_LOGFMT(Log_AssetManagedLoaderHelper, Verbose,
                "{Time}: {Line} {Class}: HandleDefinitionAssetLoadFailed() called for Owner={Owner} with no classes to load for Component.",
                ("Class", GET_CLASSNAME_WITH_FUNCTION),
                ("Line", GET_LINE_NUMBER),
                ("Time", GET_CURRENT_TIME),
                ("Owner", Owner ? *GetNameSafe(Owner) : TEXT("nullptr"))
            );
        }
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, Error,
            "{Time}: {Line} {Class}: HandleDefinitionAssetLoadFailed() called for Owner={Owner}",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("Owner", Owner ? *GetNameSafe(Owner) : TEXT("nullptr"))
        );
    }

    Events.OnPrimaryAssetClassNotFound.Broadcast(Owner, HasClassesToLoad);
}

TArray<FSoftObjectPath> UAssetManagedLoaderHelper::GetSoftObjectPathsFromAsset(UPreLoadingDataAsset* PreLoadingAsset)
{
    if(!PreLoadingAsset)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, Error,
            "{Time}: {Line} {Class}: GetSoftObjectPathsFromAsset() called with nullptr PreLoadingAsset.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        return TArray<FSoftObjectPath>();
	}

    if(!PreLoadingAsset->GetClass()->ImplementsInterface(UPreLoadingAssetInterface::StaticClass()))
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, Error,
            "{Time}: {Line} {Class}: PreLoadingAsset does not implement IPreLoadingAssetInterface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        return TArray<FSoftObjectPath>();
	}

    TArray<TSoftClassPtr<UObject>> AssetClasses = IPreLoadingAssetInterface::Execute_GetAssetClasses(PreLoadingAsset);
    TArray<TSoftObjectPtr<UObject>> AssetObjects = IPreLoadingAssetInterface::Execute_GetAssetObjects(PreLoadingAsset);

    if (AssetClasses.Num() == 0 && AssetObjects.Num() == 0)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, Verbose,
			"{Time}: {Line} {Class}: PreLoadingAsset has no asset classes or objects defined.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME));
        return TArray<FSoftObjectPath>();
    }
    else
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, VeryVerbose,
            "{Time}: {Line} {Class}: PreLoadingAsset has {ObjectCount} objects and {ClassCount} classes defined.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("ObjectCount", AssetObjects.Num()),
            ("ClassCount", AssetClasses.Num())
		);
    }

    TArray<FSoftObjectPath> ObjectPaths;
    for (const TSoftObjectPtr<UObject>& SoftObject : AssetObjects)
    {
        if (!SoftObject.IsNull())
        {
            ObjectPaths.Add(SoftObject.ToSoftObjectPath());
        }
    }

    TArray<FSoftObjectPath> ClassPaths;
    for (const TSoftClassPtr<UObject>& SoftClass : AssetClasses)
    {
        if (!SoftClass.IsNull())
        {
            ClassPaths.Add(SoftClass.ToSoftObjectPath());
        }
    }

    TArray<FSoftObjectPath> AllPaths = ObjectPaths;
    AllPaths.Append(ClassPaths);


    return AllPaths;
}

/**
 * @brief Loads both soft object references and soft class references asynchronously.
 *
 * This function takes two arrays of soft pointers (objects and classes), resolves their asset paths,
 * and loads them asynchronously using UAsyncAssetLoaderHelper. When all assets are loaded,
 * the delegate UAsyncAssetLoaderHelper::OnAllAssetsLoaded will be broadcast.
 *
 * @param WorldContextObject A UObject to act as the outer for the loader helper and provide context for GC.
 * @param ObjectPtrs An array of TSoftObjectPtr<UObject> referencing the objects to load.
 * @param ClassPtrs An array of TSoftClassPtr<UObject> referencing the classes to load.
 * @return A valid pointer to the loader helper instance if initiated successfully, or nullptr if setup failed.
 *
 * @note The returned loader will manage its own lifetime (via AddToRoot / RemoveFromRoot).
 */
UAsyncAssetLoaderHelper* UAssetManagedLoaderHelper::LoadObjectsAndClassesAsync(
    UObject* WorldContextObject,
    const TArray<TSoftObjectPtr<UObject>>& ObjectPtrs,
    const TArray<TSoftClassPtr<UObject>>& ClassPtrs
)
{
    if (!WorldContextObject)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, Warning,
            "{Time}: {Line} {Class}: LoadObjectsAndClassesAsync called with null WorldContextObject.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return nullptr;
    }

    // --- Create the loader instance ---
    UAsyncAssetLoaderHelper* Loader = NewObject<UAsyncAssetLoaderHelper>(WorldContextObject);
    if (!Loader)
    {
        UE_LOGFMT(Log_AssetManagedLoaderHelper, Error,
            "{Time}: {Line} {Class}: Failed to create UAsyncAssetLoaderHelper instance.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME)
        );
        return nullptr;
    }

    Loader->AddToRoot();

    UE_LOGFMT(Log_AssetManagedLoaderHelper, Verbose,
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
            UE_LOGFMT(Log_AssetManagedLoaderHelper, VeryVerbose,
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
            UE_LOGFMT(Log_AssetManagedLoaderHelper, VeryVerbose,
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

    UE_LOGFMT(Log_AssetManagedLoaderHelper, Verbose,
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
    Loader->StartLoading(PathsToLoad);

    UE_LOGFMT(Log_AssetManagedLoaderHelper, Verbose,
        "{Time}: {Line} {Class}: UAsyncAssetLoaderHelper::StartLoading called with {Count} path(s).",
        ("Class", GET_CLASSNAME_WITH_FUNCTION),
        ("Line", GET_LINE_NUMBER),
        ("Time", GET_CURRENT_TIME),
        ("Count", PathsToLoad.Num())
    );

    return Loader;
}