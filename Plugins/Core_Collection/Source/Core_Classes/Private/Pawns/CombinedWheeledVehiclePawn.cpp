// This file is part of the CombinedWheeledVehiclePawn implementation in the Elemental project.

#include "Pawns/CombinedWheeledVehiclePawn.h"

#include "AssetManager/AssetManagedLoaderHelper.h"
#include "AssetManager/AsyncAssetLoaderHelper.h"

#include "Helper/AssetLoader/HandleAssetLoaded.h"
#include "Helper/EngineAssignmentDataHelper.h"

#include "Components/ArrowComponent.h"
#include "Components/GameFrameworkComponentManager.h"

#include "DataAssets/DebugArrowSetupDataAsset.h"
#include "DataAssets/SpawnPointDefinitionDataAsset.h"
#include "DataAssets/SkeletalMeshSetupDataAsset.h"
#include "DataAssets/ChaosVehicleSetupDataAsset.h"

#include "Interfaces/InteractInterface.h"
#include "Interfaces/ShapeUpdatedInterface.h"

#include "Libs/CoreInformationFunctionLibrary.h"

#include "Logging/Core_Classes_Logging.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Structs/InformationSet.h"

#include "Subsystems/SpawnInstanceSubsystem.h"
#include "Subsystems/InteractManageGameInstanceSubsystem.h"

#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"

#include "TimerManager.h"

#if WITH_EDITOR
static bool bLoadCombinedBundleVehiclePawn = true;
static bool bLogCombinedBundleVehiclePawn = false;
#endif

ACombinedWheeledVehiclePawn::ACombinedWheeledVehiclePawn()
{
	IgnoredGameFeatures = TArray<FString>();

	bActorDespawnsOnPossessOtherPawn = false;
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
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: Forward component is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
	
	ArrowConfigurationAsset = nullptr;

	DataAssetIdentifier = FName(TEXT("BaseActorDefinition"));

	GetMovementComponent()->bAutoRegisterUpdatedComponent =false;
}

void ACombinedWheeledVehiclePawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	if(ArrowConfigurationAsset.IsNull())
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Log, "{Time}: {Line} {Class}: ArrowConfigurationAsset is null, using default configuration.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
	else
	{
		if (!ArrowConfigurationAsset.IsValid())
		{
			ArrowConfigurationAsset.LoadSynchronous();
		}

		Forward->SetRelativeTransform(ArrowConfigurationAsset.Get()->RelativeTransform);
		UE_LOGFMT(Log_CombinedVehiclePawns, Log, "{Time}: {Line} {Class}: ArrowConfigurationAsset loaded successfully with RelativeTransform: {Transform}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Transform", ArrowConfigurationAsset.Get()->RelativeTransform.ToString()));
	}
#endif
	FSoftObjectPath AssetPath("/Core_Collection/Data/DA_EngineSetup_Default.DA_EngineSetup_Default");
	TSoftObjectPtr<class UChaosVehicleSetupDataAsset> DefaultEngineSetup = Cast<UChaosVehicleSetupDataAsset>(AssetPath.TryLoad());
	
	if (!DefaultEngineSetup)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: DefaultEngineSetup is null, Please configure the correct asset path: /Core_Collection/Data/DA_EngineSetup_Default.DA_EngineSetup_Default",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	DefaultEngineSetup.LoadSynchronous();

	if (!DefaultEngineSetup.IsValid())
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: DefaultEngineSetup is not valid, cannot apply setup.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UChaosWheeledVehicleMovementComponent* WheeledVehicleMovementComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
	if(!WheeledVehicleMovementComponent)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: VehicleMovementComponent is null, cannot apply setup.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if(WheeledVehicleMovementComponent->WheelSetups.Num() > 0)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: VehicleMovementComponent has wheel setup with wheel setup, cannot apply default setup.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));

		return;
	}

	if(WheeledVehicleMovementComponent->EngineSetup.TorqueCurve.ExternalCurve != nullptr)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: VehicleMovementComponent already has an engine setup with torque curve, cannot apply default setup.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UChaosVehicleSetupDataAsset* EngineSetup = DefaultEngineSetup.Get();

	if(!EngineSetup)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: EngineSetup is null, cannot apply setup.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	FEngineAssignmentDataHelper::ApplyData(EngineSetup, WheeledVehicleMovementComponent);

	UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: Successfully applied vehicle setup data from DefaultEngineSetup.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
}
void ACombinedWheeledVehiclePawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}
void ACombinedWheeledVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: CombinedWheeledVehiclePawn BeginPlay called.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));

	AddGameFeatureListener();

	LoadPrimaryAssets();

	// Bind interaction callback if not already bound
	if (!GetOnInteractWithPawn().IsBound())
	{
		GetOnInteractWithPawn().AddDynamic(this, &ACombinedWheeledVehiclePawn::InteractedWithPawn);
	}

	// Bind interaction callback if not already bound
	if (!GetOnStopInteractingWithPawn().IsBound())
	{
		GetOnStopInteractingWithPawn().AddDynamic(this, &ACombinedWheeledVehiclePawn::StoppedInteractWithPawn);
	}
}
void ACombinedWheeledVehiclePawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveGameFeatureListener();
	
	Super::EndPlay(EndPlayReason);
	
	UnbindAssetLoaderEventListener();

	GetOnInteractWithPawn().RemoveAll(this);
	GetOnStopInteractingWithPawn().RemoveAll(this);

	UE_LOGFMT(Log_CombinedVehiclePawns, VeryVerbose, "{Time}: {Line} {Class}: CombinedWheeledVehiclePawn EndPlay called.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
}

void ACombinedWheeledVehiclePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: CombinedWheeledVehiclePawn PossessedBy called for {Owner} by {Controller}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Controller", *NewController->GetName()));
	if (NewController)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: PossessedByController called for {Owner} by {Controller}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", GetFName().ToString()),
			("Controller", *NewController->GetName()));

		OnPossessedByController.Broadcast(NewController);
	}
	else
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: PossessedBy called with null Controller.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
}

EPawnControlType ACombinedWheeledVehiclePawn::GetPawnControlType_Implementation() const
{
	return EPawnControlType::PCT_VEHICLE;
}

// Spawns a default pawn, optionally using an exit transform
void ACombinedWheeledVehiclePawn::SpawnDefaultPawn()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: GameInstance is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	USpawnInstanceSubsystem* SpawnInstanceSubsystem = GameInstance->GetSubsystem<USpawnInstanceSubsystem>();
	if (!SpawnInstanceSubsystem)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: SpawnInstanceSubsystem is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: PlayerController is null.",
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

FTransform ACombinedWheeledVehiclePawn::GetSpawnPointForController_Implementation(AController* InController) const
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
void ACombinedWheeledVehiclePawn::ControllerPossessesPawn_Implementation(AController* InController)
{
	if (InController == nullptr)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: Possessed byController called with null Controller.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
	UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: PossessedByController called for {Owner} by {Controller}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Controller", *InController->GetName()));

	OnPossessedByController.Broadcast(InController);
}
#pragma endregion Possess

#pragma region Interact
// Called when another actor interacts with this pawn (e.g. entering)
void ACombinedWheeledVehiclePawn::InteractedWithPawn(AActor* Interactor, AController* InteractorInstigator)
{
	if(!Interactor)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: Interactor is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if(!InteractorInstigator)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: InteractorInstigator is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: InteractedWithPawn called for {Owner} by {Interactor}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Interactor", *Interactor->GetName()));

	if (UInteractManageGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UInteractManageGameInstanceSubsystem>())
	{
		Subsystem->InteractedWithActor(this, Interactor, InteractorInstigator);
		UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: Interacted with actor {Owner} by {Interactor} handled by UInteractManageGameInstanceSubsystem.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", GetFName().ToString()),
			("Interactor", *Interactor->GetName()));
	}
}
void ACombinedWheeledVehiclePawn::StoppedInteractWithPawn(AActor* Interactor, AController* InteractorInstigator)
{
	if (!Interactor)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: Interactor is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!InteractorInstigator)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: InteractorInstigator is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: InteractedWithPawn called for {Owner} by {Interactor}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Interactor", *Interactor->GetName()));

	if (UInteractManageGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UInteractManageGameInstanceSubsystem>())
	{
		Subsystem->StoppedInteractWithActor(this, Interactor, InteractorInstigator);
		UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: Stopped Interact with actor {Owner} by {Interactor} handled by UInteractManageGameInstanceSubsystem.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", GetFName().ToString()),
			("Interactor", *Interactor->GetName()));
	}
}
void ACombinedWheeledVehiclePawn::InteractOnPawn_Implementation(AController* InteractorInstigator)
{
	if (!InteractorInstigator)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: InteractOnPawn called with null Controller.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: [EXIT] InteractOnPawn called for {Owner} by {Controller}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()),
		("Controller", *InteractorInstigator->GetName()));

	if (UInteractManageGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UInteractManageGameInstanceSubsystem>())
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: [EXIT] Delegating to InteractManageGameInstanceSubsystem->InteractedWithActor (exit vehicle).",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		Subsystem->InteractedWithActor(this, this, InteractorInstigator);
	}
	else
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: [EXIT BLOCKED] UInteractManageGameInstanceSubsystem is null, cannot exit.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
}
#pragma endregion Interact

#pragma region SceneComponentCatchInterface
USceneComponent* ACombinedWheeledVehiclePawn::GetSceneComponent_Implementation(const FName& ComponentName) const
{
	if (ComponentName == FName("Forward") && Forward)
	{
		return Forward;
	}
	else if (ComponentName == FName("Mesh") && GetMesh())
	{
		return GetMesh();
	}
	return GetRootComponent();
}
#pragma endregion SceneComponentCatchInterface

#pragma region GameFeatures
bool ACombinedWheeledVehiclePawn::AddGameFeatureListener()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
		this,
		UGameFrameworkComponentManager::NAME_GameActorReady
	);
	return true;
}
bool ACombinedWheeledVehiclePawn::RemoveGameFeatureListener()
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	return true;
}
bool ACombinedWheeledVehiclePawn::ShouldIgnoreGameFeature_Implementation(const FString& GameFeature) const
{
	// Log the check for ignoring a game feature
	UE_LOGFMT(Log_CombinedVehiclePawns, VeryVerbose, "{Time}: {Line} {Class}: CombinedWheeledVehiclePawn should ignore game feature {GameFeature} : {Result}",
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
void ACombinedWheeledVehiclePawn::SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject)
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
			UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: Loaded object is not a USpawnPointDefinitionDataAsset.",
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
				UE_LOGFMT(Log_CombinedVehiclePawns, VeryVerbose, "{Time}: {Line} {Class}: SpawnPointDefinitionDataAsset {AssetName} is not of type ESpawnPointType::SPT_ENTER.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("AssetName", *SpawnPointDefinitionData->GetName()));
				continue;
			}

			if (Collection.SpawnPoints.Num() == 0)
			{
				UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: No spawn points defined in USpawnPointDefinitionDataAsset.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME));
			}

			SpawnPoints = Collection.SpawnPoints;

			UE_LOGFMT(Log_CombinedVehiclePawns, VeryVerbose, "{Time}: {Line} {Class}: Successfully loaded spawn points from USpawnPointDefinitionDataAsset {AssetName} with {Num} Points.",
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
			UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: No valid spawn points found in USpawnPointDefinitionDataAsset {AssetName}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *SpawnPointDefinitionData->GetName()));
		}
	}
	else if (PrimaryAssetId == UMeshSetupDataAsset::GetAssetType())
	{
		USkeletalMeshSetupDataAsset* MeshSetupData = Cast<USkeletalMeshSetupDataAsset>(LoadedObject);

		if (!MeshSetupData)
		{
			UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: Loaded object is not a USkeletalMeshSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		if (MeshSetupData->Mesh.IsNull())
		{
			UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: MeshSetupDataAsset {AssetName} has no valid mesh.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *MeshSetupData->GetName()));
			return;
		}

		if (MeshSetupData->AnimInstance.IsNull())
		{
			UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: MeshSetupDataAsset {AssetName} has no valid AnimInstance.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *MeshSetupData->GetName()));
		}

		SetMeshAndAnimInstance(MeshSetupData->Mesh, MeshSetupData->AnimInstance);
	}
	else if (PrimaryAssetId == UChaosVehicleSetupDataAsset::GetAssetType())
	{
		UChaosVehicleSetupDataAsset* VehicleSetupData = Cast<UChaosVehicleSetupDataAsset>(LoadedObject);

		if (!VehicleSetupData)
		{
			UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: Loaded object is not a UElementalChaosVehicleSetupDataAsset",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}
		UChaosWheeledVehicleMovementComponent* Component = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());

		if (!Component)
		{
			UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: ChaosVehicleMovementComponent is null",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}
		
		Component->SetComponentTickEnabled(false);
		Component->Deactivate();

		FEngineAssignmentDataHelper::ApplyData(VehicleSetupData, Component);
			UE_LOGFMT(Log_CombinedVehiclePawns, Log, "{Time}: {Line} {Class}: Successfully applied vehicle setup data from loaded object: {AssetName} for Actor {Actor}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *VehicleSetupData->GetName()),
				("Actor", GetFName()));

		Component->RecreatePhysicsState(); // 💡 wichtig
		Component->Activate();
		Component->SetComponentTickEnabled(true);
	}
}
void ACombinedWheeledVehiclePawn::SetMeshAndAnimInstance(TSoftObjectPtr<class USkeletalMesh> SkeletalMesh, TSoftClassPtr<class UAnimInstance> AnimInstance)
{
	UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: LoadMeshAndAnimInstance called for {Owner}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()));

	UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: Setting SkeletalMesh and AnimInstance for {Owner}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()));

	if(!SkeletalMesh.IsValid())
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: SkeletalMesh is not valid.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	GetMesh()->SetSkeletalMesh(SkeletalMesh.Get());

	if(!AnimInstance.IsValid())
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: AnimInstance is not valid.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
	else
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(AnimInstance.Get());

		UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: SkeletalMesh and AnimInstance set successfully.",
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
			UE_LOGFMT(Log_CombinedVehiclePawns, Verbose, "{Time}: {Line} {Class}: Shape updated for component {Component}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Component", *Component->GetName()));
		}
	}
}
void ACombinedWheeledVehiclePawn::LoadPrimaryAssets()
{
#if WITH_EDITOR
	if (!bLoadCombinedBundleVehiclePawn)
	{
		UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: LoadPrimaryAssets() is disabled in editor settings.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
#endif
	// --- Binding der statischen Delegates ---
	UE_LOGFMT(Log_CombinedVehiclePawns, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: LoadPrimaryAssets() called. {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Name", GetFName().ToString()));

	UE_LOGFMT(Log_CombinedVehiclePawns, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: DataAsset Identifier: {DataAssetIdentifier}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("DataAssetIdentifier", DataAssetIdentifier.ToString()));

	// --- Binding der statischen Delegates ---
	UE_LOGFMT(Log_CombinedVehiclePawns, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: Binding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	// --- Binding der statischen Delegates ---
	AssetLoaderEvents.OnPrimaryAssetLoaded.AddDynamic(
		this, &ACombinedWheeledVehiclePawn::SetupLoadedAsset);

	// Kick off
	UE_LOGFMT(Log_CombinedVehiclePawns, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: Starting to load actor definition config asset.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	UAssetManagedLoaderHelper::LoadDefinitionConfigAssets(
		AssetLoaderEvents,
		EAssetDefinitionType::ADT_ACTOR,
		this
	);
}
void ACombinedWheeledVehiclePawn::UnbindAssetLoaderEventListener()
{
	// Unbind static delegates
	UE_LOGFMT(Log_CombinedVehiclePawns, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: Unbinding static delegates.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	// Unbind static delegates
	AssetLoaderEvents.OnPrimaryAssetLoaded.RemoveDynamic(
		this, &ACombinedWheeledVehiclePawn::SetupLoadedAsset);
}
FName ACombinedWheeledVehiclePawn::GetDefinitionAssetName_Implementation() const
{
	return DataAssetIdentifier;
}
void ACombinedWheeledVehiclePawn::SetDefinitionAssetName_Implementation(FName AssetName)
{
	DataAssetIdentifier = AssetName;
	UE_LOGFMT(Log_CombinedVehiclePawns, VeryVerbose, "[BulkScripts/AssetLoader/HandleAssetLoader]{Time}: {Line} {Class}: SetDefinitionAssetName called with AssetName: {AssetName}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("AssetName", AssetName.ToString()));
}
#pragma endregion AssetLoader


#pragma region "CoreInformation Editor"
void ACombinedWheeledVehiclePawn::SetEntityCoreLoggingEnabled_Implementation(const bool bEnabled)
{
	bLogCoreInformation = bEnabled;

	UE_LOGFMT(Log_CombinedActors, VeryVerbose, "{Time}: {Line} {Class}: SetEntity Core Logging Enabled: {bEnabled}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("bEnabled", bEnabled ? TEXT("True") : TEXT("False")));

}
FName ACombinedWheeledVehiclePawn::GetCoreInformationName_Implementation() const
{
	return "Actor";
}

TArray<FInformationSet> ACombinedWheeledVehiclePawn::GetSettingsVariableSets_Implementation() const
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