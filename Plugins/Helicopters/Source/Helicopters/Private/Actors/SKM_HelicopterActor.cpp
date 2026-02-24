// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SKM_HelicopterActor.h"

#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Interfaces/AssetTypeInterface.h"
#include "Interfaces/ShapeUpdatedInterface.h"
#include "DataAssets/MeshSetupDataAsset.h"
#include "DataAssets/SkeletalMeshSetupDataAsset.h"

#include "Logging/GlobalLog.h"
#include "Logging/Logging_Helicopters.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(Log_HelicopterActor);
ASKM_HelicopterActor::ASKM_HelicopterActor()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelicopterMesh"));
	RootComponent = Mesh;

	Forward->SetupAttachment(Mesh);
}

USceneComponent* ASKM_HelicopterActor::GetSceneComponent_Implementation(const FName& ComponentName) const
{
	if (ComponentName == TEXT("Mesh") || ComponentName == TEXT("Collision"))
	{
		return Mesh;
	}
	return Super::GetSceneComponent_Implementation(ComponentName);
}

void ASKM_HelicopterActor::SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject)
{
	Super::SetupLoadedAsset(InOwner, LoadedObject);

	if (!InOwner)
	{
		UE_LOGFMT(Log_HelicopterActor, Error, "{Time}: {Line} {Class}: Owner is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (Cast<AActor>(InOwner) != this)
	{
		UE_LOGFMT(Log_HelicopterActor, Error, "{Time}: {Line} {Class}: Owner is not this actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!LoadedObject)
	{
		UE_LOGFMT(Log_HelicopterActor, Error, "{Time}: {Line} {Class}: Loaded object is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}

	if (!LoadedObject->GetClass()->ImplementsInterface(UAssetTypeInterface::StaticClass()))
	{
		UE_LOGFMT(Log_HelicopterActor, Error, "{Time}: {Line} {Class}: Loaded object does not implement UAssetTypeInterface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	FPrimaryAssetType PrimaryAssetId = IAssetTypeInterface::Execute_GetAssetType(LoadedObject);

	//if (PrimaryAssetId == USpawnPointDefinitionDataAsset::GetAssetType())
	//{
	//	USpawnPointDefinitionDataAsset* SpawnPointDefinitionData = Cast<USpawnPointDefinitionDataAsset>(LoadedObject);
	//	if (!SpawnPointDefinitionData)
	//	{
	//		UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: Loaded object is not a USpawnPointDefinitionDataAsset.",
	//			("Class", GET_CLASSNAME_WITH_FUNCTION),
	//			("Line", GET_LINE_NUMBER),
	//			("Time", GET_CURRENT_TIME));
	//		return;
	//	}

	//	bool found = false;

	//	for (const auto& Collection : SpawnPointDefinitionData->SpawnPoints)
	//	{

	//		if (Collection.SpawnPointType != ESpawnPointType::SPT_ENTER)
	//		{
	//			UE_LOGFMT(Log_CombinedVehiclePawns, VeryVerbose, "{Time}: {Line} {Class}: SpawnPointDefinitionDataAsset {AssetName} is not of type ESpawnPointType::SPT_ENTER.",
	//				("Class", GET_CLASSNAME_WITH_FUNCTION),
	//				("Line", GET_LINE_NUMBER),
	//				("Time", GET_CURRENT_TIME),
	//				("AssetName", *SpawnPointDefinitionData->GetName()));
	//			continue;
	//		}

	//		if (Collection.SpawnPoints.Num() == 0)
	//		{
	//			UE_LOGFMT(Log_CombinedVehiclePawns, Warning, "{Time}: {Line} {Class}: No spawn points defined in USpawnPointDefinitionDataAsset.",
	//				("Class", GET_CLASSNAME_WITH_FUNCTION),
	//				("Line", GET_LINE_NUMBER),
	//				("Time", GET_CURRENT_TIME));
	//		}

	//		SpawnPoints = Collection.SpawnPoints;

	//		UE_LOGFMT(Log_CombinedVehiclePawns, VeryVerbose, "{Time}: {Line} {Class}: Successfully loaded spawn points from USpawnPointDefinitionDataAsset {AssetName} with {Num} Points.",
	//			("Class", GET_CLASSNAME_WITH_FUNCTION),
	//			("Line", GET_LINE_NUMBER),
	//			("Time", GET_CURRENT_TIME),
	//			("AssetName", *SpawnPointDefinitionData->GetName()),
	//			("Num", SpawnPoints.Num()));

	//		found = true;
	//		break; // Exit the loop after finding the first valid spawn point collection
	//	}

	//	if (!found)
	//	{
	//		UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: No valid spawn points found in USpawnPointDefinitionDataAsset {AssetName}.",
	//			("Class", GET_CLASSNAME_WITH_FUNCTION),
	//			("Line", GET_LINE_NUMBER),
	//			("Time", GET_CURRENT_TIME),
	//			("AssetName", *SpawnPointDefinitionData->GetName()));
	//	}
	//}
	//else
	if (PrimaryAssetId == UMeshSetupDataAsset::GetAssetType())
	{
		USkeletalMeshSetupDataAsset* MeshSetupData = Cast<USkeletalMeshSetupDataAsset>(LoadedObject);

		if (!MeshSetupData)
		{
			UE_LOGFMT(Log_HelicopterActor, Error, "{Time}: {Line} {Class}: Loaded object is not a USkeletalMeshSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		if (MeshSetupData->Mesh.IsNull())
		{
			UE_LOGFMT(Log_HelicopterActor, Error, "{Time}: {Line} {Class}: MeshSetupDataAsset {AssetName} has no valid mesh.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *MeshSetupData->GetName()));
			return;
		}

		if (MeshSetupData->AnimInstance.IsNull())
		{
			UE_LOGFMT(Log_HelicopterActor, Warning, "{Time}: {Line} {Class}: MeshSetupDataAsset {AssetName} has no valid AnimInstance.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *MeshSetupData->GetName()));
		}

		SetMeshAndAnimInstance(MeshSetupData->Mesh, MeshSetupData->AnimInstance);
	}
	//else if (PrimaryAssetId == UChaosVehicleSetupDataAsset::GetAssetType())
	//{
	//	UChaosVehicleSetupDataAsset* VehicleSetupData = Cast<UChaosVehicleSetupDataAsset>(LoadedObject);

	//	if (!VehicleSetupData)
	//	{
	//		UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: Loaded object is not a UElementalChaosVehicleSetupDataAsset",
	//			("Class", GET_CLASSNAME_WITH_FUNCTION),
	//			("Line", GET_LINE_NUMBER),
	//			("Time", GET_CURRENT_TIME));
	//		return;
	//	}
	//	UChaosWheeledVehicleMovementComponent* Component = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());

	//	if (!Component)
	//	{
	//		UE_LOGFMT(Log_CombinedVehiclePawns, Error, "{Time}: {Line} {Class}: ChaosVehicleMovementComponent is null",
	//			("Class", GET_CLASSNAME_WITH_FUNCTION),
	//			("Line", GET_LINE_NUMBER),
	//			("Time", GET_CURRENT_TIME));
	//		return;
	//	}

	//	Component->SetComponentTickEnabled(false);
	//	Component->Deactivate();

	//	FEngineAssignmentDataHelper::ApplyData(VehicleSetupData, Component);
	//	UE_LOGFMT(Log_CombinedVehiclePawns, Log, "{Time}: {Line} {Class}: Successfully applied vehicle setup data from loaded object: {AssetName} for Actor {Actor}",
	//		("Class", GET_CLASSNAME_WITH_FUNCTION),
	//		("Line", GET_LINE_NUMBER),
	//		("Time", GET_CURRENT_TIME),
	//		("AssetName", *VehicleSetupData->GetName()),
	//		("Actor", GetFName()));

	//	Component->RecreatePhysicsState(); // 💡 wichtig
	//	Component->Activate();
	//	Component->SetComponentTickEnabled(true);
	//}
}

void ASKM_HelicopterActor::SetMeshAndAnimInstance(TSoftObjectPtr<class USkeletalMesh> SkeletalMesh, TSoftClassPtr<class UAnimInstance> AnimInstance)
{
	UE_LOGFMT(Log_HelicopterActor, Verbose, "{Time}: {Line} {Class}: LoadMeshAndAnimInstance called for {Owner}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()));

	UE_LOGFMT(Log_HelicopterActor, Verbose, "{Time}: {Line} {Class}: Setting SkeletalMesh and AnimInstance for {Owner}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()));

	if (!SkeletalMesh.IsValid())
	{
		UE_LOGFMT(Log_HelicopterActor, Error, "{Time}: {Line} {Class}: SkeletalMesh is not valid.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	Mesh->SetSkeletalMesh(SkeletalMesh.Get());

	if (!AnimInstance.IsValid())
	{
		UE_LOGFMT(Log_HelicopterActor, Warning, "{Time}: {Line} {Class}: AnimInstance is not valid.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
	else
	{
		Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		Mesh->SetAnimInstanceClass(AnimInstance.Get());

		UE_LOGFMT(Log_HelicopterActor, Verbose, "{Time}: {Line} {Class}: SkeletalMesh and AnimInstance set successfully.",
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
			UE_LOGFMT(Log_HelicopterActor, Verbose, "{Time}: {Line} {Class}: Shape updated for component {Component}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Component", *Component->GetName()));
		}
	}
}