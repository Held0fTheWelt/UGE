// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/SKM_HelicopterPawn.h"

#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Interfaces/AssetTypeInterface.h"
#include "Interfaces/ShapeUpdatedInterface.h"
#include "DataAssets/MeshSetupDataAsset.h"
#include "DataAssets/SkeletalMeshSetupDataAsset.h"

#include "Logging/GlobalLog.h"
#include "Logging/Logging_Helicopters.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(Log_SKMHelicopterPawn);

ASKM_HelicopterPawn::ASKM_HelicopterPawn()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelicopterMesh"));
	RootComponent = Mesh;

	Forward->SetupAttachment(Mesh);
}

USceneComponent* ASKM_HelicopterPawn::GetSceneComponent_Implementation(const FName& ComponentName) const
{
	if (ComponentName == TEXT("Mesh") || ComponentName == TEXT("Collision"))
	{
		return Mesh;
	}
	return Super::GetSceneComponent_Implementation(ComponentName);
}


void ASKM_HelicopterPawn::SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject)
{
	Super::SetupLoadedAsset(InOwner, LoadedObject);

	if (!InOwner)
	{
		UE_LOGFMT(Log_SKMHelicopterPawn, Error, "{Time}: {Line} {Class}: Owner is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (Cast<AActor>(InOwner) != this)
	{
		UE_LOGFMT(Log_SKMHelicopterPawn, Error, "{Time}: {Line} {Class}: Owner is not this actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!LoadedObject)
	{
		UE_LOGFMT(Log_SKMHelicopterPawn, Error, "{Time}: {Line} {Class}: Loaded object is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}

	if (!LoadedObject->GetClass()->ImplementsInterface(UAssetTypeInterface::StaticClass()))
	{
		UE_LOGFMT(Log_SKMHelicopterPawn, Error, "{Time}: {Line} {Class}: Loaded object does not implement UAssetTypeInterface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	FPrimaryAssetType PrimaryAssetId = IAssetTypeInterface::Execute_GetAssetType(LoadedObject);

	if (PrimaryAssetId == UMeshSetupDataAsset::GetAssetType())
	{
		USkeletalMeshSetupDataAsset* MeshSetupData = Cast<USkeletalMeshSetupDataAsset>(LoadedObject);

		if (!MeshSetupData)
		{
			UE_LOGFMT(Log_SKMHelicopterPawn, Error, "{Time}: {Line} {Class}: Loaded object is not a USkeletalMeshSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		if (MeshSetupData->Mesh.IsNull())
		{
			UE_LOGFMT(Log_SKMHelicopterPawn, Error, "{Time}: {Line} {Class}: MeshSetupDataAsset {AssetName} has no valid mesh.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *MeshSetupData->GetName()));
			return;
		}

		if (MeshSetupData->AnimInstance.IsNull())
		{
			UE_LOGFMT(Log_SKMHelicopterPawn, Warning, "{Time}: {Line} {Class}: MeshSetupDataAsset {AssetName} has no valid AnimInstance.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("AssetName", *MeshSetupData->GetName()));
		}

		SetMeshAndAnimInstance(MeshSetupData->Mesh, MeshSetupData->AnimInstance);
	}
}

void ASKM_HelicopterPawn::SetMeshAndAnimInstance(TSoftObjectPtr<class USkeletalMesh> SkeletalMesh, TSoftClassPtr<class UAnimInstance> AnimInstance)
{
	UE_LOGFMT(Log_SKMHelicopterPawn, Verbose, "{Time}: {Line} {Class}: LoadMeshAndAnimInstance called for {Owner}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()));

	UE_LOGFMT(Log_SKMHelicopterPawn, Verbose, "{Time}: {Line} {Class}: Setting SkeletalMesh and AnimInstance for {Owner}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Owner", GetFName().ToString()));

	if (!SkeletalMesh.IsValid())
	{
		UE_LOGFMT(Log_SKMHelicopterPawn, Error, "{Time}: {Line} {Class}: SkeletalMesh is not valid.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	Mesh->SetSkeletalMesh(SkeletalMesh.Get());

	if (!AnimInstance.IsValid())
	{
		UE_LOGFMT(Log_SKMHelicopterPawn, Warning, "{Time}: {Line} {Class}: AnimInstance is not valid.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
	else
	{
		Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		Mesh->SetAnimInstanceClass(AnimInstance.Get());

		UE_LOGFMT(Log_SKMHelicopterPawn, Verbose, "{Time}: {Line} {Class}: SkeletalMesh and AnimInstance set successfully.",
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
			UE_LOGFMT(Log_SKMHelicopterPawn, Verbose, "{Time}: {Line} {Class}: Shape updated for component {Component}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Component", *Component->GetName()));
		}
	}
}