// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CombinedBundleActor.h"

#include "AssetManager/AsyncAssetLoaderHelper.h"
#include "AssetManager/AssetManagedLoaderHelper.h"

#include "Helper/AssetLoader/HandleAssetLoaded.h"

#include "Components/ArrowComponent.h"
#include "Components/GameFrameworkComponentManager.h"

#include "Libs/CoreInformationFunctionLibrary.h"

#include "Logging/Core_Classes_Logging.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Structs/InformationSet.h"

#include "Subsystems/InteractManageGameInstanceSubsystem.h"

#if WITH_EDITOR
static bool bLoadCombinedBundleActor = true;
static bool bLogCombinedBundleActor = false;
#endif

ACombinedBundleActor::ACombinedBundleActor(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	Forward = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward"));

	Forward->SetRelativeLocation(FVector::ZeroVector);
	Forward->SetRelativeRotation(FRotator::ZeroRotator);
	IgnoredGameFeatures = TArray<FString>();
	DataAssetIdentifier = FName(TEXT("BaseActorDefinition"));
	PrimaryAssetNamesToLoad = { FName(TEXT("BaseActorDefinition")) };
}

void ACombinedBundleActor::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ACombinedBundleActor::BeginPlay()
{
	Super::BeginPlay();

	AddGameFeatureListener();

	LoadPrimaryAssets();

	// Bind interaction callback if not already bound
	if (!GetOnInteractWithPawn().IsBound())
	{
		GetOnInteractWithPawn().AddDynamic(this, &ACombinedBundleActor::InteractedWithPawn);
	}

	// Bind interaction callback if not already bound
	if (!GetOnStopInteractingWithPawn().IsBound())
	{
		GetOnStopInteractingWithPawn().AddDynamic(this, &ACombinedBundleActor::StoppedInteractWithPawn);
	}
}
void ACombinedBundleActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	RemoveGameFeatureListener();

	UnbindAssetLoaderEventListener();
}

#pragma region SceneComponentCatchInterface
USceneComponent* ACombinedBundleActor::GetSceneComponent_Implementation(const FName& ComponentName) const
{
	return GetOwner()->GetRootComponent();
}
#pragma endregion SceneComponentCatchInterface

#pragma region Interact
void ACombinedBundleActor::InteractedWithPawn(AActor* Interactor, AController* InteractorInstigator)
{
	if (!Interactor)
	{
		UE_LOGFMT(Log_CombinedActors, Warning, "{Time}: {Line} {Class}: Interactor is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!InteractorInstigator)
	{
		UE_LOGFMT(Log_CombinedActors, Warning, "{Time}: {Line} {Class}: InteractorInstigator is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_CombinedActors, Verbose, "{Time}: {Line} {Class}: InteractedWithPawn called for {Owner} by {Interactor}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Interactor", *Interactor->GetName()));

	if (UInteractManageGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UInteractManageGameInstanceSubsystem>())
	{
		Subsystem->InteractedWithActor(this, Interactor, InteractorInstigator);
		UE_LOGFMT(Log_CombinedActors, Verbose, "{Time}: {Line} {Class}: Interacted with actor {Owner} by {Interactor} handled by UInteractManageGameInstanceSubsystem.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", GetFName().ToString()),
			("Interactor", *Interactor->GetName()));
	}
}
void ACombinedBundleActor::StoppedInteractWithPawn(AActor* Interactor, AController* InteractorInstigator)
{
	if (!Interactor)
	{
		UE_LOGFMT(Log_CombinedActors, Warning, "{Time}: {Line} {Class}: Interactor is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!InteractorInstigator)
	{
		UE_LOGFMT(Log_CombinedActors, Warning, "{Time}: {Line} {Class}: InteractorInstigator is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_CombinedActors, Verbose, "{Time}: {Line} {Class}: InteractedWithPawn called for {Owner} by {Interactor}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Interactor", *Interactor->GetName()));

	if (UInteractManageGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UInteractManageGameInstanceSubsystem>())
	{
		Subsystem->StoppedInteractWithActor(this, Interactor, InteractorInstigator);
		UE_LOGFMT(Log_CombinedActors, Verbose, "{Time}: {Line} {Class}: Stopped Interact with actor {Owner} by {Interactor} handled by UInteractManageGameInstanceSubsystem.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", GetFName().ToString()),
			("Interactor", *Interactor->GetName()));
	}
}
#pragma endregion Interact

#pragma region GameFeatures
bool ACombinedBundleActor::AddGameFeatureListener()
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
	UE_LOGFMT(Log_CombinedActors, Verbose, "{Time}: {Line} {Class}: Added game feature listener to CombinedBundleActor: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));
	return true;
}
bool ACombinedBundleActor::RemoveGameFeatureListener()
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
	UE_LOGFMT(Log_CombinedActors, Verbose, "{Time}: {Line} {Class}: Removed game feature listener from CombinedBundleActor: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));
	return true;
}
bool ACombinedBundleActor::ShouldIgnoreGameFeature_Implementation(const FString& GameFeature) const
{
	// Log the check for ignoring a game feature
	UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: CombinedBundleActor should ignore game feature {GameFeature} : {Result}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("GameFeature", GameFeature),
		("Result", IgnoredGameFeatures.Contains(GameFeature) ? TEXT("True") : TEXT("False"))
	);
	return IgnoredGameFeatures.Contains(GameFeature);
}
#pragma endregion GameFeatures

void ACombinedBundleActor::LoadPrimaryAssets()
{
	// --- Binding der statischen Delegates ---
	UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: LoadPrimaryAssets() called. {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));

	UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: DataAsset Identifier: {DataAssetIdentifier}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("DataAssetIdentifier", DataAssetIdentifier.ToString()));

	// --- Binding der statischen Delegates ---
	UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: Binding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	AssetLoaderEvents.OnPrimaryAssetLoaded.AddDynamic(
		this, &ACombinedBundleActor::SetupLoadedAsset);

	// Kick off
	UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: Starting to load actor definition config asset.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	UAssetManagedLoaderHelper::LoadDefinitionConfigAssets(
		AssetLoaderEvents,
		EAssetDefinitionType::ADT_ACTOR,
		this
	);
}

void ACombinedBundleActor::SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject)
{
	if(!InOwner)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: Owner is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (Cast<ACombinedBundleActor>(InOwner) != this)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: Owner is not this actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!LoadedObject)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: Loaded object is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
}

void ACombinedBundleActor::UnbindAssetLoaderEventListener()
{
	// Unbind static delegates
	UE_LOGFMT(Log_CombinedActors, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: Unbinding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	AssetLoaderEvents.OnPrimaryAssetLoaded.RemoveDynamic(
		this, &ACombinedBundleActor::SetupLoadedAsset);
}

FName ACombinedBundleActor::GetDefinitionAssetName_Implementation() const
{
	return DataAssetIdentifier;
}

void ACombinedBundleActor::SetDefinitionAssetName_Implementation(FName AssetName)
{
	DataAssetIdentifier = AssetName;
	UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: SetDefinitionAssetName called with AssetName: {AssetName}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("AssetName", AssetName.ToString()));
}

#pragma region "CoreInformation Editor"
void ACombinedBundleActor::SetEntityCoreLoggingEnabled_Implementation(const bool bEnabled)
{
	bLogCoreInformation = bEnabled;

	UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: SetEntity Core Logging Enabled: {bEnabled}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("bEnabled", bEnabled ? TEXT("True") : TEXT("False")));

}
FName ACombinedBundleActor::GetCoreInformationName_Implementation() const
{
	return "Actor";
}

TArray<FInformationSet> ACombinedBundleActor::GetSettingsVariableSets_Implementation() const
{
	TArray<FInformationSet> InformationSets;

	FInformationSet GameFeatureInformationSet = UCoreInformationFunctionLibrary::GetInformationSet("Game Features",
		"Game Features to Ignore", 
		"Settings for Game Features, that are normally added to this class, but are ignored on this instance");


	if(IgnoredGameFeatures.Num() == 0)
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

	FInformationSet PrimaryAssetInformationSet = UCoreInformationFunctionLibrary::GetInformationSet(
		"",
		"Primary Asset Names to Load",
		"The primary asset names that are loaded by this actor"
	);

	for (const FName& PrimaryAssetName : PrimaryAssetNamesToLoad)
	{
		UCoreInformationFunctionLibrary::AddInformationValue(
			PrimaryAssetInformationSet,
			PrimaryAssetName.ToString(),
			"",
			EValueType::VT_STRING
		);
	}

	InformationSets.Add(PrimaryAssetInformationSet);

	return InformationSets;
}
#pragma endregion "CoreInformation Editor"