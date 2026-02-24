// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/DefinedCollectionSpawningActor.h"

#include "AssetManager/AsyncAssetLoaderHelper.h"
#include "AssetManager/AssetManagedLoaderHelper.h"

#include "Components/ChildActorComponent.h"

#include "DataAssets/ActorCollectionDataAsset.h"

#include "Engine/AssetManager.h"

#include "Interfaces/AssetTypeInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Logging/Core_Classes_Logging.h"

#include "Subsystems/SpawnInstanceSubsystem.h"
#include "Subsystems/InteractManageGameInstanceSubsystem.h"
#include "Structs/EntityDefinition.h"

#if WITH_EDITOR
static bool bLoadCombinedBundleDefinedPawn = true;
static bool bLogCombinedBundleDefinedPawn = false;
#endif

ADefinedCollectionSpawningActor::ADefinedCollectionSpawningActor()
{
	PrimaryActorTick.bCanEverTick = false;

	DefinedActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("DefinedActor"));
	RootComponent = DefinedActor;

	DefinedActorClassName = FEntityDefinition();
	SpawnType = ECollectionSpawnType::CST_NONE;
#if WITH_EDITORONLY_DATA
	//DefinedCollection = nullptr;
#endif
}

void ADefinedCollectionSpawningActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	if (DefinedActor && !DefinedCollection.IsNull())
	{
		DefinedCollection.LoadSynchronous();

		if(!DefinedCollection.IsValid())
		{
			UE_LOGFMT(Log_DefinedCollectionSpawner, Error, "{Time}: {Line} {Class}: DefinedCollection is not valid. Cannot set DummyActor.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
			return;
		}

		if (!DefinedCollection->DummyActor.IsNull())
		{
			DefinedCollection->DummyActor.LoadSynchronous();
			DefinedActor->SetChildActorClass(DefinedCollection->DummyActor.Get());
		}
		else
		{
			UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: DummyActor is null in TestAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		}
	}	
	else
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: DefinedActor is null or DefinedCollection is not set as TestAsset.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
	}
#endif
}

void ADefinedCollectionSpawningActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UnbindAssetLoaderEventListener();
}

void ADefinedCollectionSpawningActor::UnbindAssetLoaderEventListener()
{
	// Unbind static delegates
	UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: Unbinding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	AssetLoaderEvents.OnPrimaryAssetLoaded.RemoveDynamic(
		this, &ADefinedCollectionSpawningActor::OnPrimaryAssetLoaded);
}

void ADefinedCollectionSpawningActor::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	if (DefinedActor->GetChildActor())
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Verbose, "{Time}: {Line} {Class}: Child actor already exists, destroying it to load async with AssetManager. ",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		UE_LOGFMT(Log_DefinedCollectionSpawner, Verbose, "{Time}: {Line} {Class}: Editor only feature to run async instead. ",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		DefinedActor->DestroyChildActor();
	}
#endif

	if (DefinedActorClassName.SelectedEntity == FName("None"))
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: DefinedActorClassName is set to 'None'. Please set a valid class name in the editor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	if (SpawnType == ECollectionSpawnType::CST_NONE)
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: SpawnType is set to 'None'. Please set a valid spawn type in the editor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	LoadPrimaryAssets();
}


void ADefinedCollectionSpawningActor::LoadPrimaryAssets()
{
#if WITH_EDITOR
	if (!bLoadCombinedBundleDefinedPawn)
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: LoadPrimaryAssets() is disabled in editor settings.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
#endif
	// --- Binding der statischen Delegates ---
	UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: LoadPrimaryAssets() called. {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));

	UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: DataAsset Identifier: {DataAssetIdentifier}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("DataAssetIdentifier", DefinedActorClassName.SelectedEntity.ToString()));

	// --- Binding der statischen Delegates ---
	UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: Binding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	AssetLoaderEvents.OnPrimaryAssetLoaded.AddDynamic(
		this, &ADefinedCollectionSpawningActor::OnPrimaryAssetLoaded);

	// Kick off
	UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: Starting to load actor definition config asset.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));

	TArray<FName> AssetNamesToLoad = { UActorCollectionDataAsset::GetAssetType() };

	UAssetManagedLoaderHelper::LoadDefinitionConfigAssets(
		AssetLoaderEvents,
		EAssetDefinitionType::ADT_ACTOR,
		this
		//AssetNamesToLoad,
		//DefinedActorClassName.SelectedEntity, this
	);
}

void ADefinedCollectionSpawningActor::OnPrimaryAssetLoaded(UObject* InOwner, UObject* LoadedObject)
{
	if (!InOwner)
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: Owner is null, cannot process loaded asset.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (Cast<AActor>(InOwner) != this)
	{
		UE_LOGFMT(Log_CombinedCharacters, Error, "{Time}: {Line} {Class}: Owner is not this actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!LoadedObject)
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: LoadedObject is null, cannot process loaded asset.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}

	if (!LoadedObject->GetClass()->ImplementsInterface(UAssetTypeInterface::StaticClass()))
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Error, "{Time}: {Line} {Class}: Loaded object does not implement UAssetTypeInterface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: PrimaryAssetLoaded called for {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", LoadedObject->GetFName().ToString()));
	
	FPrimaryAssetType PrimaryAssetId = IAssetTypeInterface::Execute_GetAssetType(LoadedObject);
	if (PrimaryAssetId == UActorCollectionDataAsset::GetAssetType())
	{
		UActorCollectionDataAsset* Asset = Cast<UActorCollectionDataAsset>(LoadedObject);
		if (!Asset)
		{
			UE_LOGFMT(Log_DefinedCollectionSpawner, Error, "{Time}: {Line} {Class}: Loaded object is not a UActorCollectionDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		switch (SpawnType)
		{
		case ECollectionSpawnType::CST_DUMMY:
			if (Asset->DummyActor.IsNull())
			{
				UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: Dummy Actor is null in LoadedAsset. Cannot spawn",
					("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
				return;
			}
			else
			{
				UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: Dummy Actor is valid. Spawning defined actor.",
					("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
				LoadDefinedActor(Asset);
			}
			break;
		case ECollectionSpawnType::CST_DEFAULT:
			if (Asset->DefaultActor.IsNull())
			{
				UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: Default Actor is null in LoadedAsset. Cannot spawn",
					("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
				return;
			}
			else
			{
				UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: Default Actor is valid. Spawning defined actor.",
					("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
				LoadDefinedActor(Asset);
			}
			break;
		case ECollectionSpawnType::CST_DESTROYED:
			if (Asset->DestroyedActor.IsNull())
			{
				UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: Destroyed Actor is null in LoadedAsset. Cannot spawn",
					("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
				return;
			}
			else
			{
				UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: Destroyed Actor is valid. Spawning defined actor.",
					("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
				LoadDefinedActor(Asset);
			}
			break;
		case ECollectionSpawnType::CST_VARIANT:
			UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: CST_VARIANT is not implemented yet. Cannot spawn.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
			break;
		default:
			break;
		}
	}
	//
	//UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: PrimaryAssetLoaded called for {Name}",
	//	("Class", GET_CLASSNAME_WITH_FUNCTION),
	//	("Line", GET_LINE_NUMBER),
	//	("Time", GET_CURRENT_TIME),
	//	("Name", LoadedObject->GetFName().ToString()));
}

void ADefinedCollectionSpawningActor::LoadDefinedActor(UActorCollectionDataAsset* Asset)
{
	if (!Asset)
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Error, "{Time}: {Line} {Class}: Collection is null. Cannot load defined actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	switch (SpawnType)
	{
	case ECollectionSpawnType::CST_DUMMY:
		if (Asset->DummyActor.IsValid())
		{
			UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: Dummy Actor is valid. Spawning defined actor.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
			SpawnDefinedActor(Asset);
			return;
		}
		break;
	case ECollectionSpawnType::CST_DEFAULT:
		if (Asset->DefaultActor.IsValid())
		{
			UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: Default Actor is valid. Spawning defined actor.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
			SpawnDefinedActor(Asset);
			return;
		}
		break;
	case ECollectionSpawnType::CST_DESTROYED:
		if (Asset->DestroyedActor.IsValid())
		{
			UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: Destroyed Actor is valid. Spawning defined actor.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
			SpawnDefinedActor(Asset);
			return;
		}
		break;
	case ECollectionSpawnType::CST_VARIANT:
		UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: CST_VARIANT is not implemented yet. Cannot spawn.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		break;
	default:
		break;
	}
}

void ADefinedCollectionSpawningActor::SpawnDefinedActor(UActorCollectionDataAsset* Asset)
{
	if (!Asset)
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Error, "{Time}: {Line} {Class}: Collection is null. Cannot load defined actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

	DefinedCollection = Asset;

	UGameInstance* GameInstance = GetGameInstance();

	if (!GameInstance)
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Error,
			"{Time}: {Line} {Class}: GameInstance is null. Cannot spawn defined actor: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", *this->GetFName().ToString()));
		return;
	}

	TSubclassOf<AActor> ActorClass = nullptr;

	switch (SpawnType)
	{
	case ECollectionSpawnType::CST_DUMMY:
		ActorClass = Asset->DummyActor.Get();
		break;
	case ECollectionSpawnType::CST_DEFAULT:
		ActorClass = Asset->DefaultActor.Get();
		break;
	case ECollectionSpawnType::CST_DESTROYED:
		ActorClass = Asset->DestroyedActor.Get();
		break;
	case ECollectionSpawnType::CST_VARIANT:
		UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: CST_VARIANT is not implemented yet. Cannot spawn.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		break;
	default:
		break;
	}

	if(ActorClass == nullptr)
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Error,
			"{Time}: {Line} {Class}: ActorClass is null. Cannot spawn defined actor: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", *this->GetFName().ToString()));
		return;
	}

	if (USpawnInstanceSubsystem* Subsystem = GameInstance->GetSubsystem<USpawnInstanceSubsystem>())
	{
		AActor* Actor = Subsystem->SpawnDefaultActorDeferred(ActorClass.Get(), GetActorTransform());

		if(Actor->GetClass()->ImplementsInterface(UAssetDefinitionInformationInterface::StaticClass()))
		{
			IAssetDefinitionInformationInterface::Execute_SetDefinitionAssetName(Actor, DefinedActorClassName.SelectedEntity);
			UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: SetDefinitionAssetName {Name} on Actor: {Actor}",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("Name", *DefinedActorClassName.SelectedEntity.ToString()), ("Actor", Actor->GetFName().ToString()));
		}
		else
		{
			UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: Actor does not implement IAssetDefinitionInformationInterface. Cannot set DefinitionAssetName.",
				("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		}

		if(SpawnType == ECollectionSpawnType::CST_DUMMY)
		{
			//Actor->SetActorLabel(FString::Printf(TEXT("Dummy %s"), *DefinedActorClassName.SelectedEntity.ToString()));
		}
		else if(SpawnType == ECollectionSpawnType::CST_DEFAULT)
		{
			//Actor->SetActorLabel(FString::Printf(TEXT("Default %s"), *DefinedActorClassName.SelectedEntity.ToString()));
		}
		else if(SpawnType == ECollectionSpawnType::CST_DESTROYED)
		{
			//Actor->SetActorLabel(FString::Printf(TEXT("Destroyed %s"), *DefinedActorClassName.SelectedEntity.ToString()));
		}

		Actor->FinishSpawning(GetActorTransform());

		AddCollectionToSpawningSystem();

		Destroy();
	}
	else
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Error, "{Time}: {Line} {Class}: SpawnInstanceSubsystem is not available in GameInstance. Cannot spawn defined actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
	}
}

void ADefinedCollectionSpawningActor::AddCollectionToSpawningSystem()
{
	UGameInstance* GameInstance = GetGameInstance();

	if (!GameInstance)
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Error,
			"{Time}: {Line} {Class}: GameInstance is null. Cannot add spawn definition for actor: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", *this->GetFName().ToString()));
		return;
	}

	UInteractManageGameInstanceSubsystem* InteractManageSystem = GameInstance->GetSubsystem<UInteractManageGameInstanceSubsystem>();

	if (!InteractManageSystem)
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Error,
			"{Time}: {Line} {Class}: InteractManageSystem is null. Cannot add spawn definition for actor: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", *this->GetFName().ToString()));
		return;
	}
	
	InteractManageSystem->AddPrimaryDataAsset(DefinedCollection);
}

FName ADefinedCollectionSpawningActor::GetDefinitionAssetName_Implementation() const
{
	return DefinedActorClassName.SelectedEntity;
}
