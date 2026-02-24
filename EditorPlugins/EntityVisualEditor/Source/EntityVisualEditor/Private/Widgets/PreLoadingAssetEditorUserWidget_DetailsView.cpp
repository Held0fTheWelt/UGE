// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PreLoadingAssetEditorUserWidget.h"

#include "Components/DetailsView.h"

#include "DataAssets/ActorCollectionDataAsset.h"
#include "DataAssets/ActorUIImageDataAsset.h"
#include "DataAssets/AircraftLightSetupDataAsset.h"
#include "DataAssets/AnimatedTrackDataAsset.h"
#include "DataAssets/ArrowComponentDefinitionDataAsset.h"
#include "DataAssets/BarrelConfigDataAsset.h"
#include "DataAssets/CameraSetupDataAsset.h"
#include "DataAssets/ChaosVehicleSetupDataAsset.h"
#include "DataAssets/CollisionBoxConfigDataAsset.h"
#include "DataAssets/CollisionControlDataAsset.h"
#include "DataAssets/EngineSoundDataAsset.h"
#include "DataAssets/FactionInformationDataAsset.h"
#include "DataAssets/HelicopterSetupDataAsset.h"
#include "DataAssets/HelipadConfigurationDataAsset.h"
#include "DataAssets/MainMeshRelativeTranslationDataAsset.h"
#include "DataAssets/MissileConfigurationDataAsset.h"
#include "DataAssets/PhysicsBasedHelicopterDefinitionDataAsset.h"
#include "DataAssets/PreLoadingDataAsset.h"
#include "DataAssets/ProjectileConfigDataAsset.h"
#include "DataAssets/ReplacementActorDataAsset.h"
#include "DataAssets/SceneComponentCatchConfigDataAsset.h"
#include "DataAssets/SpawnPointDefinitionDataAsset.h"
#include "DataAssets/SkeletalMeshSetupDataAsset.h"
#include "DataAssets/StaticMeshAnimationDataAsset.h"
#include "DataAssets/StaticMeshCollectionSetupDataAsset.h"
#include "DataAssets/StaticMeshSetupDataAsset.h"
#include "DataAssets/Texture2DCollectionDataAsset.h"
#include "DataAssets/TrainingTargetConfigDataAsset.h"
#include "DataAssets/VehicleLightsSetupDataAsset.h"
#include "DataAssets/VehiclePhysicsSetupDataAsset.h"
#include "DataAssets/Weapon_GatlingGunConfigDataAsset.h"
#include "DataAssets/Weapon_HybridGunConfigDataAsset.h"
#include "DataAssets/Weapon_MissileLauncherConfigDataAsset.h"
#include "DataAssets/WidgetComponentSetupDataAsset.h"
#include "DataAssets/WidgetDefinitionDataAsset.h"

#include "Interfaces/AssetTypeInterface.h"

#include "Libs/EntityCoreEditorFunctionLibrary.h"

#include "Logging/GlobalLog.h"
#include "Logging/Logging_Editor_VisualEditor.h"
#include "Logging/StructuredLog.h"

#include "EditorUtilityWidgetComponents.h"

void UPreLoadingAssetEditorUserWidget::NativeSetDetailsViewData()
{
	if (!PreLoadingDataAsset)
	{
		UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: List item object is not a valid PreLoadingDataAsset.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}
	else
	{
		UE_LOGFMT(Log_PreLoadingAssetEditor, Display, "{Time}: {Line} {Class}: Setting details view data for asset: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", PreLoadingDataAsset ? PreLoadingDataAsset->GetName() : TEXT("null"))
		);
	}

	if(!DetailsView)
	{
		UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: DetailsView is null. Cannot set details view data.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}

	DetailsView->bForceHiddenPropertyVisibility = true; // Hide all properties by default

	if(!PreLoadingDataAsset->GetClass()->ImplementsInterface(UAssetTypeInterface::StaticClass()))
	{
		UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: PreLoadingDataAsset does not implement IAssetTypeInterface. Cannot set details view data.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME)
		);
		return;
	}

#pragma region Elemental Collection Data Assets
	if(IAssetTypeInterface::Execute_GetAssetType(PreLoadingDataAsset) == UActorUIImageDataAsset::GetAssetType())
	{
		UActorUIImageDataAsset* ActorUIImageDataAsset = Cast<UActorUIImageDataAsset>(PreLoadingDataAsset);

		if(!ActorUIImageDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UActorUIImageDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}

		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("UI"));
		DetailsView->SetObject(ActorUIImageDataAsset);

		UE_LOGFMT(Log_PreLoadingAssetEditor, Display, "{Time}: {Line} {Class}: Set details view data for UActorUIImageDataAsset: {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Name", ActorUIImageDataAsset->GetName())
		);
	}
	else if(PreLoadingDataAsset->IsA(UAnimatedTrackDataAsset::StaticClass()))
	{
		UAnimatedTrackDataAsset* AnimatedTrackDataAsset = Cast<UAnimatedTrackDataAsset>(PreLoadingDataAsset);
		if(!AnimatedTrackDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UAnimatedTrackDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Tracks"));
		DetailsView->SetObject(AnimatedTrackDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UArrowComponentDefinitionDataAsset::StaticClass()))
	{
		UArrowComponentDefinitionDataAsset* ArrowComponentDefinitionDataAsset = Cast<UArrowComponentDefinitionDataAsset>(PreLoadingDataAsset);
		if(!ArrowComponentDefinitionDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UArrowComponentDefinitionDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Arrow Components"));
		DetailsView->SetObject(ArrowComponentDefinitionDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UBarrelConfigDataAsset::StaticClass()))
	{
		UBarrelConfigDataAsset* BarrelConfigDataAsset = Cast<UBarrelConfigDataAsset>(PreLoadingDataAsset);
		if(!BarrelConfigDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UBarrelConfigDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Barrel Configuration"));
		DetailsView->SetObject(BarrelConfigDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UCameraSetupDataAsset::StaticClass()))
	{
		UCameraSetupDataAsset* CameraSetupDataAsset = Cast<UCameraSetupDataAsset>(PreLoadingDataAsset);
		if(!CameraSetupDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UCameraSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Camera Setup"));
		DetailsView->CategoriesToShow.Add(TEXT("Skeletal Mesh Component"));
		DetailsView->CategoriesToShow.Add(TEXT("Post Process"));
		DetailsView->SetObject(CameraSetupDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UChaosVehicleSetupDataAsset::StaticClass()))
	{
		UChaosVehicleSetupDataAsset* ChaosVehicleSetupDataAsset = Cast<UChaosVehicleSetupDataAsset>(PreLoadingDataAsset);
		if(!ChaosVehicleSetupDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UElementalChaosVehicleSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("WheelSetup"));
		DetailsView->CategoriesToShow.Add(TEXT("Custom"));
		DetailsView->CategoriesToShow.Add(TEXT("MechanicalSetup"));
		DetailsView->CategoriesToShow.Add(TEXT("SteeringSetup"));
		DetailsView->CategoriesToShow.Add(TEXT("Vehicle"));
		DetailsView->CategoriesToShow.Add(TEXT("VehicleSetup"));
		DetailsView->CategoriesToShow.Add(TEXT("AerofoilSetup"));
		DetailsView->CategoriesToShow.Add(TEXT("ThrusterSetup"));
		DetailsView->CategoriesToShow.Add(TEXT("ArcadeControl"));
		DetailsView->CategoriesToShow.Add(TEXT("VehicleInput"));
		DetailsView->SetObject(ChaosVehicleSetupDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UCollisionBoxConfigDataAsset::StaticClass()))
	{
		UCollisionBoxConfigDataAsset* CollisionBoxConfigDataAsset = Cast<UCollisionBoxConfigDataAsset>(PreLoadingDataAsset);
		if(!CollisionBoxConfigDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UCollisionBoxConfigDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Collision"));
		DetailsView->SetObject(CollisionBoxConfigDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UMainMeshRelativeTranslationDataAsset::StaticClass()))
	{
		UMainMeshRelativeTranslationDataAsset* MainMeshRelativeTranslationDataAsset = Cast<UMainMeshRelativeTranslationDataAsset>(PreLoadingDataAsset);
		if(!MainMeshRelativeTranslationDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UMainMeshRelativeTranslationDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Transform"));
		DetailsView->SetObject(MainMeshRelativeTranslationDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(USceneComponentCatchConfigDataAsset::StaticClass()))
	{
		USceneComponentCatchConfigDataAsset* SceneComponentCatchConfigDataAsset = Cast<USceneComponentCatchConfigDataAsset>(PreLoadingDataAsset);
		if(!SceneComponentCatchConfigDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to USceneComponentCatchConfigDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Catch Config"));
		DetailsView->SetObject(SceneComponentCatchConfigDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(USkeletalMeshSetupDataAsset::StaticClass()))
	{
		USkeletalMeshSetupDataAsset* SkeletalMeshSetupDataAsset = Cast<USkeletalMeshSetupDataAsset>(PreLoadingDataAsset);
		if(!SkeletalMeshSetupDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to USkeletalMeshSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Mesh"));
		DetailsView->SetObject(SkeletalMeshSetupDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(USpawnPointDefinitionDataAsset::StaticClass()))
	{
		USpawnPointDefinitionDataAsset* SpawnPointDefinitionDataAsset = Cast<USpawnPointDefinitionDataAsset>(PreLoadingDataAsset);
		if(!SpawnPointDefinitionDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to USpawnPointDefinitionDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Spawn Points"));
		DetailsView->SetObject(SpawnPointDefinitionDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UStaticMeshCollectionSetupDataAsset::StaticClass()))
	{
		UStaticMeshCollectionSetupDataAsset* StaticMeshCollectionSetupDataAsset = Cast<UStaticMeshCollectionSetupDataAsset>(PreLoadingDataAsset);
		if(!StaticMeshCollectionSetupDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UStaticMeshCollectionSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Collection"));
		DetailsView->SetObject(StaticMeshCollectionSetupDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UStaticMeshSetupDataAsset::StaticClass()))
	{
		UStaticMeshSetupDataAsset* StaticMeshSetupDataAsset = Cast<UStaticMeshSetupDataAsset>(PreLoadingDataAsset);
		if(!StaticMeshSetupDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UStaticMeshSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Mesh"));
		DetailsView->SetObject(StaticMeshSetupDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UTexture2DCollectionDataAsset::StaticClass()))
	{
		UTexture2DCollectionDataAsset* Texture2DCollectionDataAsset = Cast<UTexture2DCollectionDataAsset>(PreLoadingDataAsset);
		if(!Texture2DCollectionDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UTexture2DCollectionDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Textures"));
		DetailsView->SetObject(Texture2DCollectionDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UVehiclePhysicsSetupDataAsset::StaticClass()))
	{
		UVehiclePhysicsSetupDataAsset* VehiclePhysicsSetupDataAsset = Cast<UVehiclePhysicsSetupDataAsset>(PreLoadingDataAsset);
		if(!VehiclePhysicsSetupDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UVehiclePhysicsSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Vehicle Physics Setup"));
		DetailsView->SetObject(VehiclePhysicsSetupDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UWidgetComponentSetupDataAsset::StaticClass()))
	{
		UWidgetComponentSetupDataAsset* WidgetComponentSetupDataAsset = Cast<UWidgetComponentSetupDataAsset>(PreLoadingDataAsset);
		if(!WidgetComponentSetupDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UWidgetComponentSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Widget Setup"));
		DetailsView->SetObject(WidgetComponentSetupDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UWidgetDefinitionDataAsset::StaticClass()))
	{
		UWidgetDefinitionDataAsset* WidgetDefinitionDataAsset = Cast<UWidgetDefinitionDataAsset>(PreLoadingDataAsset);
		if(!WidgetDefinitionDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UWidgetDefinitionDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Widget"));
		DetailsView->SetObject(WidgetDefinitionDataAsset);
	}
#pragma endregion Elemental Collection Data Assets
#pragma region Core Collection Data Assets
	else if(PreLoadingDataAsset->IsA(UActorCollectionDataAsset::StaticClass()))
	{
		UActorCollectionDataAsset* ActorCollectionDataAsset = Cast<UActorCollectionDataAsset>(PreLoadingDataAsset);
		if(!ActorCollectionDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UActorCollectionDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Collection"));
		DetailsView->SetObject(ActorCollectionDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UCollisionControlDataAsset::StaticClass()))
	{
		UCollisionControlDataAsset* CollisionControlDataAsset = Cast<UCollisionControlDataAsset>(PreLoadingDataAsset);
		if(!CollisionControlDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UCollisionControlDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Collision"));
		DetailsView->SetObject(CollisionControlDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UEngineSoundDataAsset::StaticClass()))
	{
		UEngineSoundDataAsset* EngineSoundDataAsset = Cast<UEngineSoundDataAsset>(PreLoadingDataAsset);
		if(!EngineSoundDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UEngineSoundDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Audio Components"));
		DetailsView->CategoriesToShow.Add(TEXT("Sound Setup"));
		DetailsView->SetObject(EngineSoundDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UFactionInformationDataAsset::StaticClass()))
	{
		UFactionInformationDataAsset* FactionInformationDataAsset = Cast<UFactionInformationDataAsset>(PreLoadingDataAsset);
		if(!FactionInformationDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UFactionInformationDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Factions"));
		DetailsView->SetObject(FactionInformationDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UHelicopterSetupDataAsset::StaticClass()))
	{
		UHelicopterSetupDataAsset* HelicopterSetupDataAsset = Cast<UHelicopterSetupDataAsset>(PreLoadingDataAsset);
		if(!HelicopterSetupDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UHelicopterSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Setup"));
		DetailsView->SetObject(HelicopterSetupDataAsset);
	}
	else if (PreLoadingDataAsset->IsA(UReplacementActorDataAsset::StaticClass()))
		{
		UReplacementActorDataAsset* ReplacementActorDataAsset = Cast<UReplacementActorDataAsset>(PreLoadingDataAsset);
		if (!ReplacementActorDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UReplacementActorDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Replacement"));
		DetailsView->SetObject(ReplacementActorDataAsset);
	}
	else if (PreLoadingDataAsset->IsA(UStaticMeshAnimationDataAsset::StaticClass()))
	{
		UStaticMeshAnimationDataAsset* StaticMeshAnimationDataAsset = Cast<UStaticMeshAnimationDataAsset>(PreLoadingDataAsset);
		if (!StaticMeshAnimationDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UStaticMeshAnimationDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Static Mesh Animation"));
		DetailsView->SetObject(StaticMeshAnimationDataAsset);
	}
	else if (PreLoadingDataAsset->IsA(UVehicleLightsSetupDataAsset::StaticClass()))
	{
		UVehicleLightsSetupDataAsset* VehicleLightsSetupDataAsset = Cast<UVehicleLightsSetupDataAsset>(PreLoadingDataAsset);
		if (!VehicleLightsSetupDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UVehicleLightsSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Lights"));
		DetailsView->SetObject(VehicleLightsSetupDataAsset);
	}
#pragma endregion Core Collection Data Assets
#pragma region Aircraft Data Assets
	else if(PreLoadingDataAsset->IsA(UAircraftLightSetupDataAsset::StaticClass()))
	{
		UAircraftLightSetupDataAsset* AircraftLightSetupDataAsset = Cast<UAircraftLightSetupDataAsset>(PreLoadingDataAsset);
		if(!AircraftLightSetupDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UAircraftLightSetupDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Lights"));
		DetailsView->CategoriesToShow.Add(TEXT("Mesh"));
		DetailsView->CategoriesToShow.Add(TEXT("Materials"));
		DetailsView->CategoriesToShow.Add(TEXT("Color"));
		DetailsView->CategoriesToShow.Add(TEXT("Beacon"));
		DetailsView->CategoriesToShow.Add(TEXT("Navigation"));
		DetailsView->CategoriesToShow.Add(TEXT("Strobe"));
		DetailsView->CategoriesToShow.Add(TEXT("RandomPhase"));
		DetailsView->SetObject(AircraftLightSetupDataAsset);
	}
#pragma endregion Aircraft Data Assets
#pragma region Helicopter Data Assets
	else if(PreLoadingDataAsset->IsA(UPhysicsBasedHelicopterDefinitionDataAsset::StaticClass()))
	{
		UPhysicsBasedHelicopterDefinitionDataAsset* PhysicsBasedHelicopterDefinitionDataAsset = Cast<UPhysicsBasedHelicopterDefinitionDataAsset>(PreLoadingDataAsset);
		if(!PhysicsBasedHelicopterDefinitionDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UPhysicsBasedHelicopterDefinitionDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Helicopter Physics"));
		DetailsView->SetObject(PhysicsBasedHelicopterDefinitionDataAsset);
	}
#pragma endregion Helicopter Data Assets
#pragma region Warfare Data Assets
	else if(PreLoadingDataAsset->IsA(UHelipadConfigurationDataAsset::StaticClass()))
	{
		UHelipadConfigurationDataAsset* HelipadConfigurationDataAsset = Cast<UHelipadConfigurationDataAsset>(PreLoadingDataAsset);
		if(!HelipadConfigurationDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UHelipadConfigurationDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Mesh"));
		DetailsView->CategoriesToShow.Add(TEXT("Lights"));
		DetailsView->CategoriesToShow.Add(TEXT("Materials"));
		DetailsView->CategoriesToShow.Add(TEXT("Decals"));
		DetailsView->CategoriesToShow.Add(TEXT("Loop"));
		DetailsView->CategoriesToShow.Add(TEXT("Delay"));
		DetailsView->CategoriesToShow.Add(TEXT("Emissive Material Parameter"));
		DetailsView->SetObject(HelipadConfigurationDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UMissileConfigurationDataAsset::StaticClass()))
	{
		UMissileConfigurationDataAsset* MissileConfigurationDataAsset = Cast<UMissileConfigurationDataAsset>(PreLoadingDataAsset);
		if(!MissileConfigurationDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UMissileConfigurationDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Mesh"));
		DetailsView->CategoriesToShow.Add(TEXT("Relative Location"));
		DetailsView->CategoriesToShow.Add(TEXT("Collision"));
		DetailsView->CategoriesToShow.Add(TEXT("Effects"));
		DetailsView->CategoriesToShow.Add(TEXT("Sound"));
		DetailsView->CategoriesToShow.Add(TEXT("Guide Target"));
		DetailsView->CategoriesToShow.Add(TEXT("Missile Setup"));
		DetailsView->CategoriesToShow.Add(TEXT("Impact Setup"));
		DetailsView->CategoriesToShow.Add(TEXT("Projectile Movement"));
		DetailsView->CategoriesToShow.Add(TEXT("Damage"));
		DetailsView->SetObject(MissileConfigurationDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UProjectileConfigDataAsset::StaticClass()))
	{
		UProjectileConfigDataAsset* ProjectileConfigDataAsset = Cast<UProjectileConfigDataAsset>(PreLoadingDataAsset);
		if(!ProjectileConfigDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UProjectileConfigDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Projectile"));
		DetailsView->CategoriesToShow.Add(TEXT("Impact"));
		DetailsView->SetObject(ProjectileConfigDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UTrainingTargetConfigDataAsset::StaticClass()))
	{
		UTrainingTargetConfigDataAsset* TrainingTargetConfigDataAsset = Cast<UTrainingTargetConfigDataAsset>(PreLoadingDataAsset);
		if(!TrainingTargetConfigDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UTrainingTargetConfigDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Target Setup"));
		DetailsView->SetObject(TrainingTargetConfigDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UWeapon_GatlingGunConfigDataAsset::StaticClass()))
	{
		UWeapon_GatlingGunConfigDataAsset* GatlingGunConfigDataAsset = Cast<UWeapon_GatlingGunConfigDataAsset>(PreLoadingDataAsset);
		if(!GatlingGunConfigDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UWeapon_GatlingGunConfigDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Mesh"));
		DetailsView->CategoriesToShow.Add(TEXT("Effects"));
		DetailsView->CategoriesToShow.Add(TEXT("Sound"));
		DetailsView->CategoriesToShow.Add(TEXT("Projectile"));
		DetailsView->CategoriesToShow.Add(TEXT("Barrel"));
		DetailsView->CategoriesToShow.Add(TEXT("Muzzle"));
		DetailsView->SetObject(GatlingGunConfigDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UWeapon_HybridGunConfigDataAsset::StaticClass()))
	{
		UWeapon_HybridGunConfigDataAsset* HybridGunConfigDataAsset = Cast<UWeapon_HybridGunConfigDataAsset>(PreLoadingDataAsset);
		if(!HybridGunConfigDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UWeapon_HybridGunConfigDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Socket"));
		DetailsView->CategoriesToShow.Add(TEXT("Covers"));
		DetailsView->CategoriesToShow.Add(TEXT("Template"));
		DetailsView->SetObject(HybridGunConfigDataAsset);
	}
	else if(PreLoadingDataAsset->IsA(UWeapon_MissileLauncherConfigDataAsset::StaticClass()))
	{
		UWeapon_MissileLauncherConfigDataAsset* MissileLauncherConfigDataAsset = Cast<UWeapon_MissileLauncherConfigDataAsset>(PreLoadingDataAsset);
		if(!MissileLauncherConfigDataAsset)
		{
			UE_LOGFMT(Log_PreLoadingAssetEditor, Warning, "{Time}: {Line} {Class}: Failed to cast PreLoadingDataAsset to UWeapon_MissileLauncherConfigDataAsset.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME)
			);
			return;
		}
		DetailsView->CategoriesToShow.Empty();
		DetailsView->CategoriesToShow.Add(TEXT("Socket"));
		DetailsView->CategoriesToShow.Add(TEXT("Covers"));
		DetailsView->CategoriesToShow.Add(TEXT("Template"));
		DetailsView->SetObject(MissileLauncherConfigDataAsset);
	}
#pragma endregion Warfare Data Assets
	// If the PreLoadingDataAsset is not one of the specific types, set it directly
	else
	{
		DetailsView->SetObject(PreLoadingDataAsset);
	}

	SetDetailsViewData();
}
