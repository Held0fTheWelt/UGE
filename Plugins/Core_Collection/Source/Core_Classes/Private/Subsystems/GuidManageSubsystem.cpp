// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GuidManageSubsystem.h"
#include <AssetRegistry/AssetRegistryModule.h>
#include "DataAssets/GuidDataAsset.h"

#include "Logging/StructuredLog.h"
#include "Logging/GlobalLog.h"
#include "Logging/Definitions.h"

DEFINE_LOG_CATEGORY(Log_Subsystem_GuidManager);

UGuidManageSubsystem::UGuidManageSubsystem()
{
}

bool UGuidManageSubsystem::GetGuidEntryByPath(const FString& MapName, const FString& InObjectPath, FGuid& OutObjectGUID) const
{
    // 1) AssetRegistry holen
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& Registry = AssetRegistryModule.Get();

    // 2) Optional: Paths scannen, falls Registry noch nicht initialisiert ist
    Registry.ScanPathsSynchronous({ TEXT("/Game/Data") }, true);

    // 3) Alle Assets vom Typ UMapObjectDataAsset finden
    //FARFilter Filter;
    //Filter.ClassNames.Add(UGuidPrimaryDataAsset::StaticClass()->GetFName());
    //Filter.PackagePaths.Add(FName("/Game/Data"));
    //Filter.bRecursivePaths = true;

    //TArray<FAssetData> AssetList;
    //Registry.GetAssets(Filter, AssetList);

    //// 4) Finde das eine DataAsset, das du brauchst (z.B. nach Namen oder Tag)
    //UGuidPrimaryDataAsset* DataAsset = nullptr;
    //for (const FAssetData& AssetData : AssetList)
    //{
    //    // Beispiel: suche direkt nach dem Asset mit Name "MapObjectDataAsset"
    //    if (AssetData.AssetName == MapName)
    //    {
    //        DataAsset = Cast<UGuidPrimaryDataAsset>(AssetData.GetAsset());
    //        break;
    //    }
    //}

    //if (!ensure(DataAsset)) return false;

    //// 5) Jetzt wie gehabt nach ObjectPath suchen…
    ////const FString ThisPath = GetOwner()->GetPathName();
    //for (const FGuidManagerEntry& Entry : DataAsset->GuidEntries)
    //{
    //    if (Entry.ObjectPath == InObjectPath)
    //    {
    //        OutObjectGUID = Entry.ObjectGUID;
    //        //ApplySavedData(Entry);
    //        return true;
    //    }
    //}

//    // 6) Wenn noch kein Eintrag: neue GUID erzeugen…
//    FGuid NewGUID = FGuid::NewGuid();
//    OutObjectGUID = NewGUID;
//
//    // 7) Speichern der ID in der Regisry
//#if WITH_EDITOR
//    // 7) Im Editor: DataAsset modiﬁzieren und speichern
//    DataAsset->Modify();  // Begin Undo/Redo und mark dirty
//
//    FGuidManagerEntry NewEntry;
//    NewEntry.ObjectPath = InObjectPath;
//    NewEntry.ObjectGUID = NewGUID;
//    DataAsset->GuidEntries.Add(NewEntry);
//
//    // Package als dirty markieren
//    UPackage* Package = DataAsset->GetOutermost();
//    Package->SetDirtyFlag(true);
//
//    // Optional: automatisch speichern (ohne Dialog)
//    const FString PackageFileName = FPackageName::LongPackageNameToFilename(
//        Package->GetName(),
//        FPackageName::GetAssetPackageExtension()
//    );
    //UPackage::SavePackage(
    //    Package,
    //    DataAsset,
    //    EObjectFlags::RF_Public | EObjectFlags::RF_Standalone,
    //    *PackageFileName,
    //    GError,
    //    nullptr,
    //    false,
    //    true,
    //    SAVE_NoError
    //);
//#else
//    UE_LOGFMT(Log_Subsystem_GuidManager, Verbose, "{Time}: {Line} {Class}: Neuer GUID‑Eintrag für {Path} erzeugt – bitte im Editor das Asset speichern.", ("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("Path", *InObjectPath));
//#endif

    return true;
}

void UGuidManageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// Hier kannst du Initialisierungen vornehmen, die beim Start des GameInstanceSubsystems benötigt werden.
	// Zum Beispiel könntest du hier deine GUID-Daten laden oder initialisieren.
    UE_LOGFMT(Log_Subsystem_GuidManager, Warning, "{Time}: {Line} {Class}: GuidManageSubsystem initialized.", ("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
	UE_LOG(LogTemp, Log, TEXT(""));
}

void UGuidManageSubsystem::Deinitialize()
{   
	Super::Deinitialize();
	// Hier kannst du Aufräumarbeiten vornehmen, wenn das GameInstanceSubsystem beendet wird.
    UE_LOGFMT(Log_Subsystem_GuidManager, Log, "{Time}: {Line} {Class}: GuidManageSubsystem deinitialized.", ("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
}
