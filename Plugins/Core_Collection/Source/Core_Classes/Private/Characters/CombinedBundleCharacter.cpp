// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CombinedBundleCharacter.h"

#include "AssetManager/AssetManagedLoaderHelper.h"
#include "AssetManager/AsyncAssetLoaderHelper.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/GameFrameworkComponentManager.h"

#include "DataAssets/SkeletalMeshSetupDataAsset.h"

#include "Enums/AssetDefinitionType.h"

#include "Helper/AssetLoader/HandleAssetLoaded.h"

#include "Interfaces/AssetTypeInterface.h"
#include "Interfaces/InteractInterface.h"
#include "Interfaces/ShapeUpdatedInterface.h"

#include "Libs/CoreInformationFunctionLibrary.h"

#include "Logging/Core_Classes_Logging.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Structs/InformationSet.h"

#include "Subsystems/SpawnInstanceSubsystem.h"
#include "Subsystems/InteractManageGameInstanceSubsystem.h"

#if WITH_EDITOR
static bool bLoadCombinedBundleCharacter = true;
static bool bLogCombinedBundleCharacter = false;
#endif

ACombinedBundleCharacter::ACombinedBundleCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	Forward = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward"));
	Forward->SetRelativeLocation(FVector::ZeroVector);
	Forward->SetRelativeRotation(FRotator::ZeroRotator);

	if (Forward)
	{
		Forward->SetupAttachment(GetMesh());
	}
	else
	{
		UE_LOGFMT(Log_CombinedCharacters, Warning, "{Time}: {Line} {Class}: Forward component is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}

	IgnoredGameFeatures = TArray<FString>();

	bActorDespawnsOnPossessOtherPawn = false;

	DataAssetIdentifier = FName(TEXT("BaseActorDefinition"));
}

void ACombinedBundleCharacter::BeginPlay()
{
	Super::BeginPlay();

	AddGameFeatureListener();

	LoadPrimaryAssets();
}
void ACombinedBundleCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: CombinedBundleCharacter EndPlay called.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));

	RemoveGameFeatureListener();
	
	UnbindAssetLoaderEventListener();
}

#pragma region Possess
void ACombinedBundleCharacter::ControllerPossessesPawn_Implementation(AController* InController)
{
	if (InController == nullptr)
	{
		UE_LOGFMT(Log_CombinedCharacters, Warning, "{Time}: {Line} {Class}: Possessed byController called with null Controller.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
	UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: PossessedByController called for {Owner} by {Controller}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Controller", *InController->GetName()));

	OnPossessedByController.Broadcast(InController);
}
#pragma endregion Possess

#pragma region Interact
void ACombinedBundleCharacter::InteractedWithPawn(AActor* Interactor, AController* InteractorInstigator)
{
	if (!Interactor)
	{
		UE_LOGFMT(Log_CombinedCharacters, Warning, "{Time}: {Line} {Class}: Interactor is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!InteractorInstigator)
	{
		UE_LOGFMT(Log_CombinedCharacters, Warning, "{Time}: {Line} {Class}: InteractorInstigator is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: InteractedWithPawn called for {Owner} by {Interactor}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Interactor", *Interactor->GetName()));

	if (UInteractManageGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UInteractManageGameInstanceSubsystem>())
	{
		Subsystem->InteractedWithActor(this, Interactor, InteractorInstigator);
		UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: Interacted with actor {Owner} by {Interactor} handled by UInteractManageGameInstanceSubsystem.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", GetFName().ToString()),
			("Interactor", *Interactor->GetName()));
	}
}
void ACombinedBundleCharacter::StoppedInteractWithPawn(AActor* Interactor, AController* InteractorInstigator)
{
	if (!Interactor)
	{
		UE_LOGFMT(Log_CombinedCharacters, Warning, "{Time}: {Line} {Class}: Interactor is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!InteractorInstigator)
	{
		UE_LOGFMT(Log_CombinedCharacters, Warning, "{Time}: {Line} {Class}: InteractorInstigator is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: InteractedWithPawn called for {Owner} by {Interactor}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Interactor", *Interactor->GetName()));

	if (UInteractManageGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UInteractManageGameInstanceSubsystem>())
	{
		Subsystem->StoppedInteractWithActor(this, Interactor, InteractorInstigator);
		UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: Stopped Interact with actor {Owner} by {Interactor} handled by UInteractManageGameInstanceSubsystem.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", GetFName().ToString()),
			("Interactor", *Interactor->GetName()));
	}
}
#pragma endregion Interact

#pragma region SceneComponentCatchInterface
USceneComponent* ACombinedBundleCharacter::GetSceneComponent_Implementation(const FName& ComponentName) const
{
	if (ComponentName == FName("Mesh") && GetMesh())
	{
		return GetMesh();
	}
	else if (ComponentName == FName("Collision") && GetCapsuleComponent())
	{
		UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "{Time}: {Line} {Class}: GetSceneComponent_Implementation called for Collision component.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return GetCapsuleComponent();
	}

	else if (ComponentName == FName("Forward") && Forward)
	{
		return Forward;
	}
	UE_LOGFMT(Log_CombinedCharacters, Warning, "{Time}: {Line} {Class}: GetSceneComponent_Implementation called with unknown component name: {ComponentName}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("ComponentName", ComponentName)  // FName geht direkt
	);


	return GetRootComponent();
}
#pragma endregion SceneComponentCatchInterface

#pragma region GameFeatures
bool ACombinedBundleCharacter::AddGameFeatureListener()
{
	if (bGameFeatureInitialized)
	{
		// Log that the game feature listener is not initialized
		UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "{Time}: {Line} {Class}: Game feature listener is already initialized for CombinedBundleActor: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", GetFName().ToString()));
		return false;
	}

	bGameFeatureInitialized = true;

	// Log the addition of the game feature listener
	UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "{Time}: {Line} {Class}: Adding game feature listener to CombinedBundleCharacter: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));
	if (UGameFrameworkComponentManager* GameFrameworkComponentManager = GetGameInstance()->GetSubsystem<UGameFrameworkComponentManager>())
	{
		GameFrameworkComponentManager->AddReceiver(this);
		// Log successful addition of the game feature listener
		UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: Successfully added game feature listener to CombinedBundleCharacter: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", GetFName().ToString()));
		return true;
	}
	// Log failure to add the game feature listener
	UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: Failed to add game feature listener to CombinedBundleCharacter: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));
	return false;
}
bool ACombinedBundleCharacter::RemoveGameFeatureListener()
{
	if (!bGameFeatureInitialized)
	{
		// Log that the game feature listener is not initialized
		UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "{Time}: {Line} {Class}: Game feature listener is not initialized for CombinedBundleActor: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", GetFName().ToString()));
		return false;
	}

	bGameFeatureInitialized = false;

	// Log the removal of the game feature listener
	UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "{Time}: {Line} {Class}: Removing game feature listener from CombinedBundleCharacter: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));
	if (UGameFrameworkComponentManager* GameFrameworkComponentManager = GetGameInstance()->GetSubsystem<UGameFrameworkComponentManager>())
	{
		GameFrameworkComponentManager->RemoveReceiver(this);
		// Log successful removal of the game feature listener
		UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: Successfully removed game feature listener from CombinedBundleCharacter: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", GetFName().ToString()));
		return true;
	}
	// Log failure to remove the game feature listener
	UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: Failed to remove game feature listener from CombinedBundleCharacter: {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));
	return false;
}
bool ACombinedBundleCharacter::ShouldIgnoreGameFeature_Implementation(const FString& GameFeature) const
{
	// Log the check for ignoring a game feature
	UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "{Time}: {Line} {Class}: CombinedBundleCharacter should ignore game feature {GameFeature} : {Result}",
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
void ACombinedBundleCharacter::SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject)
{
	if (!InOwner)
	{
		UE_LOGFMT(Log_CombinedCharacters, Error, "{Time}: {Line} {Class}: Owner is null.",
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
		return;
	}

	if (Cast<AActor>(InOwner) != this)
	{
		UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "{Time}: {Line} {Class}: Owner {Owner} is not this component. LoadedObjectName {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", InOwner->GetFName()),
			("Name", LoadedObject->GetFName()));
		return;
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

	if (PrimaryAssetId == UMeshSetupDataAsset::GetAssetType())
	{
		if (!bSetMeshToDefaultSkeletalMeshComponent)
		{
			UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: bSetMeshToDefaultSkeletalMeshComponent is false, skipping mesh setup.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		USkeletalMeshSetupDataAsset* MeshSetupData = Cast<USkeletalMeshSetupDataAsset>(LoadedObject);

		if (!MeshSetupData)
		{
			UE_LOGFMT(Log_CombinedCharacters, Error, "{Time}: {Line} {Class}: Loaded object is not a USkeletalMeshSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		if (MeshSetupData->Mesh.IsNull())
		{
			UE_LOGFMT(Log_CombinedCharacters, Error, "{Time}: {Line} {Class}: MeshSetupPrimaryDataAsset {AssetName} has no valid mesh.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *MeshSetupData->GetName()));
			return;
		}

		if (MeshSetupData->AnimInstance.IsNull())
		{
			UE_LOGFMT(Log_CombinedCharacters, Warning, "{Time}: {Line} {Class}: MeshSetupPrimaryDataAsset {AssetName} has no valid AnimInstance.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *MeshSetupData->GetName()));
		}


		UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: Setting SkeletalMesh and AnimInstance for {Owner}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", GetFName().ToString()));

		if (!MeshSetupData->Mesh.IsValid())
		{
			UE_LOGFMT(Log_CombinedCharacters, Error, "{Time}: {Line} {Class}: SkeletalMesh is not valid.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}
		GetMesh()->SetSkeletalMesh(MeshSetupData->Mesh.Get());

		if (!MeshSetupData->AnimInstance.IsValid())
		{
			UE_LOGFMT(Log_CombinedCharacters, Warning, "{Time}: {Line} {Class}: AnimInstance is not valid.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}
		else
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(MeshSetupData->AnimInstance.Get());

			UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: SkeletalMesh and AnimInstance set successfully.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
		}

		TArray<UActorComponent*> Components = GetComponentsByInterface(UShapeUpdatedInterface::StaticClass());

		if (Components.Num() > 0)
		{
			for (UActorComponent* Component : Components)
			{
				IShapeUpdatedInterface::Execute_ShapeUpdated(Component);
				UE_LOGFMT(Log_CombinedCharacters, Verbose, "{Time}: {Line} {Class}: Shape updated for component {Component}.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("Component", *Component->GetName()));
			}
		}
	}
}
void ACombinedBundleCharacter::LoadPrimaryAssets()
{
#if WITH_EDITOR
	if (!bLoadCombinedBundleCharacter)
	{
		UE_LOGFMT(Log_CombinedCharacters, Warning, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: LoadPrimaryAssets() is disabled in editor settings.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
#endif
	// --- Binding der statischen Delegates ---
	UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: LoadPrimaryAssets() called. {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));

	UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: DataAsset Identifier: {DataAssetIdentifier}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("DataAssetIdentifier", DataAssetIdentifier.ToString()));

	// --- Binding der statischen Delegates ---
	UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: Binding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	// --- Binding der statischen Delegates ---
	AssetLoaderEvents.OnPrimaryAssetLoaded.AddDynamic(
		this, &ACombinedBundleCharacter::SetupLoadedAsset);

	// Kick off
	UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: Starting to load actor definition config asset.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	UAssetManagedLoaderHelper::LoadDefinitionConfigAssets(
		AssetLoaderEvents,
		EAssetDefinitionType::ADT_ACTOR,
		this
	);
}
void ACombinedBundleCharacter::UnbindAssetLoaderEventListener()
{
	// Unbind static delegates
	UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: Unbinding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	// Unbind static delegates
	AssetLoaderEvents.OnPrimaryAssetLoaded.RemoveDynamic(
		this, &ACombinedBundleCharacter::SetupLoadedAsset);
}
FName ACombinedBundleCharacter::GetDefinitionAssetName_Implementation() const
{
	return DataAssetIdentifier;
}
void ACombinedBundleCharacter::SetDefinitionAssetName_Implementation(FName AssetName)
{
	DataAssetIdentifier = AssetName;
	UE_LOGFMT(Log_CombinedCharacters, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: SetDefinitionAssetName called with AssetName: {AssetName}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("AssetName", AssetName.ToString()));
}
#pragma endregion AssetLoader 


#pragma region "CoreInformation Editor"
void ACombinedBundleCharacter::SetEntityCoreLoggingEnabled_Implementation(const bool bEnabled)
{
	bLogCoreInformation = bEnabled;

	UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: SetEntity Core Logging Enabled: {bEnabled}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("bEnabled", bEnabled ? TEXT("True") : TEXT("False")));

}
FName ACombinedBundleCharacter::GetCoreInformationName_Implementation() const
{
	return "Actor";
}

TArray<FInformationSet> ACombinedBundleCharacter::GetSettingsVariableSets_Implementation() const
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