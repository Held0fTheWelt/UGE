// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/CombinedBundlePawn.h"

#include "AssetManager/AsyncAssetLoaderHelper.h"
#include "AssetManager/AssetManagedLoaderHelper.h"

#include "Helper/AssetLoader/HandleAssetLoaded.h"

#include "Components/ArrowComponent.h"
#include "Components/GameFrameworkComponentManager.h"

#include "DataAssets/SpawnPointDefinitionDataAsset.h"
#include "DataAssets/StaticMeshSetupDataAsset.h"

#include "Interfaces/AssetTypeInterface.h"
#include "Interfaces/InteractInterface.h"

#include "Libs/CoreInformationFunctionLibrary.h"

#include "Logging/Core_Classes_Logging.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Structs/InformationSet.h"

#include "Subsystems/InteractManageGameInstanceSubsystem.h"
#include "Subsystems/SpawnInstanceSubsystem.h"

#if WITH_EDITOR
static bool bLoadCombinedBundlePawn = true;

static bool bLogCombinedBundlePawn = false;
#endif

ACombinedBundlePawn::ACombinedBundlePawn()
{
	PrimaryActorTick.bCanEverTick = false;

	Forward = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward"));
	Forward->SetRelativeLocation(FVector::ZeroVector);
	Forward->SetRelativeRotation(FRotator::ZeroRotator);
	
	IgnoredGameFeatures = TArray<FString>();

	bActorDespawnsOnPossessOtherPawn = false;

	DataAssetIdentifier = FName(TEXT("BaseActorDefinition"));
}

void ACombinedBundlePawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ACombinedBundlePawn::BeginPlay()
{
	Super::BeginPlay();

	AddGameFeatureListener();

	LoadPrimaryAssets();

	// Bind interaction callback if not already bound
	if (!GetOnInteractWithPawn().IsBound())
	{
		GetOnInteractWithPawn().AddDynamic(this, &ACombinedBundlePawn::InteractedWithPawn);
	}

	// Bind interaction callback if not already bound
	if (!GetOnStopInteractingWithPawn().IsBound())
	{
		GetOnStopInteractingWithPawn().AddDynamic(this, &ACombinedBundlePawn::StoppedInteractWithPawn);
	}
}
void ACombinedBundlePawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//UE_LOGFMT(Log_CombinedPawn, Warning, "{Time}: {Line} {Class}: CombinedWheeledVehiclePawn EndPlay called.",
	//	("Class", GET_CLASSNAME_WITH_FUNCTION),
	//	("Line", GET_LINE_NUMBER),
	//	("Time", GET_CURRENT_TIME));
	RemoveGameFeatureListener();

	UnbindAssetLoaderEventListener();

	GetOnInteractWithPawn().RemoveAll(this);
	GetOnStopInteractingWithPawn().RemoveAll(this);
}

void ACombinedBundlePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOGFMT(Log_CombinedPawns, Verbose, "{Time}: {Line} {Class}: CombinedWheeledVehiclePawn PossessedBy called for {Owner} by {Controller}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Controller", *NewController->GetName()));
	if (NewController)
	{
		UE_LOGFMT(Log_CombinedPawns, Verbose, "{Time}: {Line} {Class}: PossessedByController called for {Owner} by {Controller}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", GetFName().ToString()),
			("Controller", *NewController->GetName()));

		OnPossessedByController.Broadcast(NewController);
	}
	else
	{
		UE_LOGFMT(Log_CombinedPawns, Warning, "{Time}: {Line} {Class}: PossessedBy called with null Controller.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
}

// Spawns a default pawn, optionally using an exit transform
void ACombinedBundlePawn::SpawnDefaultPawn()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		UE_LOGFMT(Log_CombinedPawns, Warning, "{Time}: {Line} {Class}: GameInstance is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	USpawnInstanceSubsystem* SpawnInstanceSubsystem = GameInstance->GetSubsystem<USpawnInstanceSubsystem>();
	if (!SpawnInstanceSubsystem)
	{
		UE_LOGFMT(Log_CombinedPawns, Warning, "{Time}: {Line} {Class}: SpawnInstanceSubsystem is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOGFMT(Log_CombinedPawns, Warning, "{Time}: {Line} {Class}: PlayerController is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	FTransform SpawnTransform;
	if (SpawnPoints.Num() > 0)
	{
		// Use first exit transform relative to this actor
		SpawnTransform = SpawnPoints[0] * GetActorTransform();
	}

	SpawnInstanceSubsystem->SpawnDefaultPawn(PlayerController, SpawnTransform);
}

FTransform ACombinedBundlePawn::GetSpawnPointForController_Implementation(AController* InController) const
{
	// initialize spawn transform to actor's transform
	FTransform SpawnTransform = GetActorTransform();
	if (SpawnPoints.Num() > 0)
	{
		// Use first exit transform relative to this actor
		SpawnTransform = SpawnPoints[0] * GetActorTransform();
	}
	return SpawnTransform;
}


#pragma region Possess
void ACombinedBundlePawn::ControllerPossessesPawn_Implementation(AController* InController)
{
	if (InController == nullptr)
	{
		UE_LOGFMT(Log_CombinedPawns, Warning, "{Time}: {Line} {Class}: Possessed byController called with null Controller.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
	UE_LOGFMT(Log_CombinedPawns, Verbose, "{Time}: {Line} {Class}: PossessedByController called for {Owner} by {Controller}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Controller", *InController->GetName()));

	OnPossessedByController.Broadcast(InController);
}
#pragma endregion Possess

#pragma region Interact
void ACombinedBundlePawn::InteractedWithPawn(AActor* Interactor, AController* InteractorInstigator)
{
	if (!Interactor)
	{
		UE_LOGFMT(Log_CombinedPawns, Warning, "{Time}: {Line} {Class}: Interactor is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!InteractorInstigator)
	{
		UE_LOGFMT(Log_CombinedPawns, Warning, "{Time}: {Line} {Class}: InteractorInstigator is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_CombinedPawns, Verbose, "{Time}: {Line} {Class}: InteractedWithPawn called for {Owner} by {Interactor}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Interactor", *Interactor->GetName()));

	if (UInteractManageGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UInteractManageGameInstanceSubsystem>())
	{
		Subsystem->InteractedWithActor(this, Interactor, InteractorInstigator);
		UE_LOGFMT(Log_CombinedPawns, Verbose, "{Time}: {Line} {Class}: Interacted with actor {Owner} by {Interactor} handled by UInteractManageGameInstanceSubsystem.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", GetFName().ToString()),
			("Interactor", *Interactor->GetName()));
	}
}
void ACombinedBundlePawn::StoppedInteractWithPawn(AActor* Interactor, AController* InteractorInstigator)
{
	if (!Interactor)
	{
		UE_LOGFMT(Log_CombinedPawns, Warning, "{Time}: {Line} {Class}: Interactor is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!InteractorInstigator)
	{
		UE_LOGFMT(Log_CombinedPawns, Warning, "{Time}: {Line} {Class}: InteractorInstigator is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_CombinedPawns, Verbose, "{Time}: {Line} {Class}: InteractedWithPawn called for {Owner} by {Interactor}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Interactor", *Interactor->GetName()));

	if (UInteractManageGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UInteractManageGameInstanceSubsystem>())
	{
		Subsystem->StoppedInteractWithActor(this, Interactor, InteractorInstigator);
		UE_LOGFMT(Log_CombinedPawns, Verbose, "{Time}: {Line} {Class}: Stopped Interact with actor {Owner} by {Interactor} handled by UInteractManageGameInstanceSubsystem.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", GetFName().ToString()),
			("Interactor", *Interactor->GetName()));
	}
}
#pragma endregion Interact

#pragma region SceneComponentCatchInterface
USceneComponent* ACombinedBundlePawn::GetSceneComponent_Implementation(const FName& ComponentName) const
{
	if (ComponentName == FName("Forward") && Forward)
	{
		return Forward;
	}
	return GetRootComponent();
}
#pragma endregion SceneComponentCatchInterface

#pragma region GameFeatures
bool ACombinedBundlePawn::AddGameFeatureListener()
{
	if (bGameFeatureInitialized)
	{
		// Log that the game feature listener is not initialized
		UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: Game feature listener is already initialized for CombinedBundleActor: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", GetFName().ToString()));
		return false;
	}

	bGameFeatureInitialized = true;

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);

	// Log addition of the game feature listener
	UE_LOGFMT(Log_CombinedPawns, Verbose, "{Time}: {Line} {Class}: Added game feature listener to CombinedBundlePawn: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));
	return true;
}
bool ACombinedBundlePawn::RemoveGameFeatureListener()
{
	if (!bGameFeatureInitialized)
	{
		// Log that the game feature listener is not initialized
		UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: Game feature listener is not initialized for CombinedBundleActor: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", GetFName().ToString()));
		return false;
	}

	bGameFeatureInitialized = false;

	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

	// Log removal of the game feature listener
	UE_LOGFMT(Log_CombinedPawns, Verbose, "{Time}: {Line} {Class}: Removed game feature listener from CombinedBundlePawn: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));
	return true;
}
bool ACombinedBundlePawn::ShouldIgnoreGameFeature_Implementation(const FString& GameFeature) const
{
	// Log the check for ignoring a game feature
	UE_LOGFMT(Log_CombinedPawns, VeryVerbose, "{Time}: {Line} {Class}: CombinedBundlePawn should ignore game feature {GameFeature} : {Result}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("GameFeature", GameFeature),
		("Result", IgnoredGameFeatures.Contains(GameFeature) ? TEXT("True") : TEXT("False"))
	);
	return IgnoredGameFeatures.Contains(GameFeature);
}
#pragma endregion GameFeatures

#pragma region AssetLoader 
void ACombinedBundlePawn::SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject)
{
	if (!InOwner)
	{
		UE_LOGFMT(Log_CombinedCharacters, Error, "{Time}: {Line} {Class}: Owner is null.",
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
		UE_LOGFMT(Log_CombinedCharacters, Error, "{Time}: {Line} {Class}: Loaded object is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}

	if (!LoadedObject->GetClass()->ImplementsInterface(UAssetTypeInterface::StaticClass()))
	{
		UE_LOGFMT(Log_CombinedCharacters, Error, "{Time}: {Line} {Class}: Loaded object does not implement UAssetTypeInterface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	FPrimaryAssetType PrimaryAssetId = IAssetTypeInterface::Execute_GetAssetType(LoadedObject);

	if (PrimaryAssetId == USpawnPointDefinitionDataAsset::GetAssetType())
	{
		USpawnPointDefinitionDataAsset* SpawnPointDefinitionData = Cast<USpawnPointDefinitionDataAsset>(LoadedObject);
		if (!SpawnPointDefinitionData)
		{
			UE_LOGFMT(Log_CombinedPawns, Error, "{Time}: {Line} {Class}: Loaded object is not a USpawnPointDefinitionDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		bool found = false;

		for (const auto& Collection : SpawnPointDefinitionData->SpawnPoints)
		{

			if (Collection.SpawnPointType != ESpawnPointType::SPT_ENTER)
			{
				UE_LOGFMT(Log_CombinedPawns, VeryVerbose, "{Time}: {Line} {Class}: SpawnPointDefinitionDataAsset {AssetName} is not of type ESpawnPointType::SPT_ENTER.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("AssetName", *SpawnPointDefinitionData->GetName()));
				continue;
			}

			if (Collection.SpawnPoints.Num() == 0)
			{
				UE_LOGFMT(Log_CombinedPawns, Warning, "{Time}: {Line} {Class}: No spawn points defined in USpawnPointDefinitionDataAsset.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME));
			}

			SpawnPoints = Collection.SpawnPoints;

			UE_LOGFMT(Log_CombinedPawns, VeryVerbose, "{Time}: {Line} {Class}: Successfully loaded spawn points from USpawnPointDefinitionDataAsset {AssetName} with {Num} Points.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *SpawnPointDefinitionData->GetName()),
				("Num", SpawnPoints.Num()));

			found = true;
			break; // Exit the loop after finding the first valid spawn point collection
		}

		if (!found)
		{
			UE_LOGFMT(Log_CombinedPawns, Error, "{Time}: {Line} {Class}: No valid spawn points found in USpawnPointDefinitionDataAsset {AssetName}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *SpawnPointDefinitionData->GetName()));
		}
	}
	else if (PrimaryAssetId == UMeshSetupDataAsset::GetAssetType())
	{
	/*	USkeletalMeshSetupPrimaryDataAsset* MeshSetupData = Cast<USkeletalMeshSetupPrimaryDataAsset>(LoadedObject);

		if (!MeshSetupData)
		{
			UE_LOGFMT(Log_CombinedPawns, Error, "[BulkScripts/AssetLoader/HandleAssetLoaded]{Time}: {Line} {Class}: Loaded object is not a USkeletalMeshSetupPrimaryDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		if (MeshSetupData->Mesh.IsNull())
		{
			UE_LOGFMT(Log_CombinedPawns, Error, "[BulkScripts/AssetLoader/HandleAssetLoaded]{Time}: {Line} {Class}: MeshSetupPrimaryDataAsset {AssetName} has no valid mesh.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *MeshSetupData->GetName()));
			return;
		}

		if (MeshSetupData->AnimInstance.IsNull())
		{
			UE_LOGFMT(Log_CombinedPawns, Warning, "[BulkScripts/AssetLoader/HandleAssetLoaded]{Time}: {Line} {Class}: MeshSetupPrimaryDataAsset {AssetName} has no valid AnimInstance.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *MeshSetupData->GetName()));
		}

		AssetsToLoad.Add(MeshSetupData->Mesh);
		ClassesToLoad.Add(MeshSetupData->AnimInstance);

		UAsyncAssetLoaderHelper* Helper = UAssetManagedLoaderHelper::LoadObjectsAndClassesAsync(
			GetWorld(),
			AssetsToLoad,
			ClassesToLoad
		);

		Helper->OnAllAssetsLoaded.AddDynamic(this, &ACombinedWheeledVehiclePawn::AllAsyncAssetsLoaded);

		DelayLoadMeshAndAnimInstance(MeshSetupData->Mesh, MeshSetupData->AnimInstance);*/
	}
}

void ACombinedBundlePawn::LoadPrimaryAssets()
{
#if WITH_EDITOR
	if (!bLoadCombinedBundlePawn)
	{
		UE_LOGFMT(Log_CombinedActors, Warning, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: LoadPrimaryAssets() is disabled in editor settings.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
#endif
	// --- Binding der statischen Delegates ---
	UE_LOGFMT(Log_CombinedPawns, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: LoadPrimaryAssets() called. {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));

	UE_LOGFMT(Log_CombinedPawns, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: DataAsset Identifier: {DataAssetIdentifier}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("DataAssetIdentifier", DataAssetIdentifier.ToString()));

	// --- Binding der statischen Delegates ---
	UE_LOGFMT(Log_CombinedPawns, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: Binding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	// --- Binding der statischen Delegates ---
	AssetLoaderEvents.OnPrimaryAssetLoaded.AddDynamic(
		this, &ACombinedBundlePawn::SetupLoadedAsset);

	// Kick off
	UE_LOGFMT(Log_CombinedPawns, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: Starting to load actor definition config asset.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	UAssetManagedLoaderHelper::LoadDefinitionConfigAssets(
		AssetLoaderEvents,
		EAssetDefinitionType::ADT_ACTOR,
		this
	);
}
void ACombinedBundlePawn::UnbindAssetLoaderEventListener()
{
	// Unbind static delegates
	UE_LOGFMT(Log_CombinedPawns, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: Unbinding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	// Unbind static delegates
	AssetLoaderEvents.OnPrimaryAssetLoaded.RemoveDynamic(
		this, &ACombinedBundlePawn::SetupLoadedAsset);
}

FName ACombinedBundlePawn::GetDefinitionAssetName_Implementation() const
{
	return DataAssetIdentifier;
}

void ACombinedBundlePawn::SetDefinitionAssetName_Implementation(FName AssetName)
{
	DataAssetIdentifier = AssetName;
	UE_LOGFMT(Log_CombinedPawns, VeryVerbose, "{Time}: {Line} {Class}: SetDefinitionAssetName called with {AssetName}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("AssetName", AssetName.ToString()));
}

#pragma region "CoreInformation Editor"
void ACombinedBundlePawn::SetEntityCoreLoggingEnabled_Implementation(const bool bEnabled)
{
	bLogCoreInformation = bEnabled;

	UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: SetEntity Core Logging Enabled: {bEnabled}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("bEnabled", bEnabled ? TEXT("True") : TEXT("False")));

}
FName ACombinedBundlePawn::GetCoreInformationName_Implementation() const
{
	return "Actor";
}

TArray<FInformationSet> ACombinedBundlePawn::GetSettingsVariableSets_Implementation() const
{
	TArray<FInformationSet> InformationSets;

	FInformationSet GameFeatureInformationSet = UCoreInformationFunctionLibrary::GetInformationSet("Game Features",
		"Game Features to Ignore",
		"Settings for Game Features, that are normally added to this class, but are ignored on this instance");


	if (IgnoredGameFeatures.Num() == 0)
	{
		UCoreInformationFunctionLibrary::AddInformationValue(
			GameFeatureInformationSet,
			"No Game Features Ignored",
			"",
			EValueType::VT_STRING
		);
	}
	else
	{
		for (const FString& GameFeature : IgnoredGameFeatures)
		{
			UCoreInformationFunctionLibrary::AddInformationValue(
				GameFeatureInformationSet,
				GameFeature,
				"",
				EValueType::VT_STRING
			);
		}
	}

	InformationSets.Add(GameFeatureInformationSet);

	FInformationSet AssetInformationSet = UCoreInformationFunctionLibrary::GetInformationSet(
		"Asset Management",
		"Data Asset Identifier",
		"The identifier for the data asset used by this actor"
	);
	UCoreInformationFunctionLibrary::AddInformationValue(
		AssetInformationSet,
		DataAssetIdentifier.ToString(),
		"",
		EValueType::VT_STRING
	);

	InformationSets.Add(AssetInformationSet);

	return InformationSets;
}
#pragma endregion "CoreInformation Editor"