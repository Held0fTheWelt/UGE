#include "Components/InteractCollisionComponent.h"

#include "Blueprint/UserWidget.h"

#include "Components/BoxComponent.h"

#include "DataAssets/ActorUIImageDataAsset.h"
#include "DataAssets/CollisionBoxConfigDataAsset.h"
#include "DataAssets/WidgetDefinitionDataAsset.h"

#include "Enums/ValueType.h"

#include "Interfaces/SceneComponentCatchInterface.h"

#include "Libs/CoreInformationFunctionLibrary.h"

#include "Logging/GF_Interact_Logging.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Structs/InformationSet.h"

void UInteractCollisionComponent::SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject)
{
	if (IsGameFeatureIgnored())
	{
		UE_LOGFMT(Log_Interact_Setup, Verbose, "{Time}: {Line} {Class}: Game feature is ignored, skipping SetupLoadedAsset on Component {ComponentName}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("ComponentName", GetNameSafe(this))
		);
		return;
	}

	if (!InOwner)
	{
		UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Owner is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!LoadedObject)
	{
		UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Loaded object is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (Cast<UActorComponent>(InOwner) != this)
	{
		UE_LOGFMT(Log_Interact_Setup, VeryVerbose, "{Time}: {Line} {Class}: Owner {Owner} is not this component. LoadedObjectName {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", InOwner->GetFName()),
			("Name", LoadedObject->GetFName()));
		return;
	}

	if (!LoadedObject->GetClass()->ImplementsInterface(UAssetTypeInterface::StaticClass()))
	{
		UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Loaded object does not implement UAssetTypeInterface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	FPrimaryAssetType PrimaryAssetId = IAssetTypeInterface::Execute_GetAssetType(LoadedObject);

	if (PrimaryAssetId == UWidgetDefinitionDataAsset::GetAssetType())
	{
		UE_LOGFMT(Log_Interact_Setup, VeryVerbose, "{Time}: {Line} {Class}: Loading WidgetDefinitionDataAsset from loaded object: {Object}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Object", LoadedObject->GetFName()));
		
		UWidgetDefinitionDataAsset* WidgetAsset = Cast<UWidgetDefinitionDataAsset>(LoadedObject);
		if (!WidgetAsset)
		{
			UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Failed to cast loaded object to UWidgetDefinitionDataAsset",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}
		
		if (WidgetAsset->WidgetClass.IsNull())
		{
			UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: WidgetClass is null in the loaded asset",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		if (!WidgetAsset->WidgetClass.IsValid())
		{
			WidgetAsset->WidgetClass.LoadSynchronous(); // Ensure the widget class is loaded synchronously
		}

		if (!WidgetAsset->WidgetClass.IsValid())
		{
			UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: WidgetClass is not valid in the loaded asset",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		WidgetTemplate = TSoftClassPtr<UUserWidget>(WidgetAsset->WidgetClass.ToSoftObjectPath());
	}

	else if (PrimaryAssetId == UCollisionBoxConfigDataAsset::GetAssetType())
	{
		UE_LOGFMT(Log_Interact_Setup, VeryVerbose, "{Time}: {Line} {Class}: Loading CollisionBoxConfigDataAsset from loaded object: {Object}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Object", LoadedObject->GetFName()));

		if (!SetCollisionComponentSetup(LoadedObject))
		{

			UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Failed to set CollisionComponentSetup from loaded object",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}
		else
		{
			UE_LOGFMT(Log_Interact_Setup, VeryVerbose, "{Time}: {Line} {Class}: Successfully set CollisionComponentSetup from loaded object",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			BuildInteractableCollisionComponents();
			return;
		}
	}

	else if (PrimaryAssetId == UActorUIImageDataAsset::GetAssetType())
	{
		UE_LOGFMT(Log_Interact_Setup, VeryVerbose, "{Time}: {Line} {Class}: Loading ActorUIImagePrimaryDataAsset from loaded object: {Object}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Object", LoadedObject->GetFName()));
		if (!SetActorUIImage(LoadedObject))
		{
			UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Failed to set ActorUIImage from loaded object",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
		}
		else
		{
			UE_LOGFMT(Log_Interact_Setup, VeryVerbose, "{Time}: {Line} {Class}: Successfully set ActorUIImage from loaded object",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
		}
		return;
	}

	Super::SetupLoadedAsset(InOwner, LoadedObject);
}

bool UInteractCollisionComponent::SetActorUIImage(UObject* Object)
{
	if (Object == nullptr)
	{
		UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Loaded object is null",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return false;
	}

	UActorUIImageDataAsset* Asset = Cast<UActorUIImageDataAsset>(Object);

	if (!Asset)
	{
		UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Failed to cast loaded object to UActorUIImageDataAsset",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return false;
	}

	if (Asset->Image.IsNull())
	{
		UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: InteractUIImage is null in the loaded asset",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return false;
	}

	// If the image is valid, set the InteractUIImage
	if (!Asset->Image.IsValid())
	{
		Asset->Image.LoadSynchronous(); // Ensure the image is loaded synchronously
	}

	if (Asset->Image.IsValid())
	{
		InteractUIImage = TSoftObjectPtr<UTexture2D>(Asset->Image.ToSoftObjectPath());
	}
	else
	{
		UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: InteractUIImage is not valid in the loaded asset",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return false;
	}

	return true;
}

bool UInteractCollisionComponent::SetCollisionComponentSetup(UObject* Object)
{
	if (Object == nullptr)
	{
		UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Loaded object is null",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return false;
	}

	UCollisionBoxConfigDataAsset* Asset = Cast<UCollisionBoxConfigDataAsset>(Object);

	if (!Asset)
	{
		UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Failed to cast loaded object to UActorUIImagePrimaryDataAsset",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return false;
	}

	if (Asset->bTurned90Degrees)
	{
		TurnedCollisionOnElement.Add(true);
		UE_LOGFMT(Log_Interact_Setup, VeryVerbose, "{Time}: {Line} {Class}: Setting turned rotation",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
	else
	{
		UE_LOGFMT(Log_Interact_Setup, VeryVerbose, "{Time}: {Line} {Class}: Setting normal rotation",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		TurnedCollisionOnElement.Add(false);
	}

	return true;
}

void UInteractCollisionComponent::BuildInteractableCollisionComponents()
{
	UE_LOGFMT(Log_Interact_Setup, VeryVerbose, "{Time}: {Line} {Class}: Building interactable collision components for GameFeature: {GameFeatureName}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("GameFeatureName", GetFName().ToString()));

	CreateBoxComponent(GetOwner()->GetRootComponent(), 0);
}

bool UInteractCollisionComponent::CreateBoxComponent(USceneComponent* Root, const int32& Index)
{
	UBoxComponent* BoxComponent = NewObject<UBoxComponent>(this);
	if (BoxComponent)
	{
		AActor* Owner = GetOwner();

		if (!Owner)
		{
			UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Owner is null. Cannot create Box component.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return false;
		}

		FBoxSphereBounds Bounds = FBoxSphereBounds();
		if (!Owner->GetClass()->ImplementsInterface(USceneComponentCatchInterface::StaticClass()))
		{
			UE_LOGFMT(Log_Interact_Setup, Warning, "{Time}: {Line} {Class}: Owner {OwnerName} does not implement SceneComponentCatchInterface. Using default (empty) bounds for collision box.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("OwnerName", *Owner->GetName()));
		}
		if (Owner->GetClass()->ImplementsInterface(USceneComponentCatchInterface::StaticClass()))
		{
			USceneComponent* Component = ISceneComponentCatchInterface::Execute_GetSceneComponent(Owner, FName("Mesh"));

			if (!Component)
			{
				UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: SceneComponentCatchInterface returned null for Mesh component.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME));
				return false;
			}

			UMeshComponent* MeshComponent = Cast<UMeshComponent>(Component);
			if (!MeshComponent)
			{
				UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: SceneComponentCatchInterface returned a non-mesh component.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME));
				return false;
			}

			Bounds = MeshComponent->GetLocalBounds();
		}

		BoxComponent->bAutoRegister = true;
		BoxComponent->RegisterComponent();
		BoxComponent->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		BoxComponent->SetBoxExtent(Bounds.BoxExtent, false);

		FVector Origin, Extents;
		Owner->GetActorBounds(true, Origin, Extents);

		BoxComponent->SetRelativeLocation(FVector(0.f, 0.f, Bounds.BoxExtent.Z));

#if WITH_EDITOR
		if (bDebugCollisionBoxComponent)
		{
			BoxComponent->SetHiddenInGame(false);
			BoxComponent->SetVisibility(true);
			bHasDebuggedCollisionBoxComponent = true;
		}
#endif


#if WITH_EDITOR
		if (bDebugMode)
		{
			// 🟩 Debug-Box zeichnen
			DrawDebugBox(
				GetWorld(),
				BoxComponent->GetComponentLocation(),				// Weltkoordinatenzentrum
				Bounds.BoxExtent,			// Halbe Abmessungen
				FQuat::Identity,	// Keine Rotation
				FColor::Green,
				false,				// Nicht dauerhaft
				0.f,				// Lebensdauer
				0,					// Depth Priority
				2.f					// Linienstärke
			);
		}
#endif
		UE_LOGFMT(Log_Interact_Setup, VeryVerbose, "{Time}: {Line} {Class}: Box component added and debug drawn",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));

		BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
		BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

		BoxComponent->RecreatePhysicsState();

		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &UInteractCollisionComponent::OnBoxComponentBeginOverlap);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &UInteractCollisionComponent::OnBoxComponentEndOverlap);

		InteractableComponents.Add(BoxComponent);
		return true;
	}
	UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Failed to create Box component", ("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
	return false;
}

void UInteractCollisionComponent::ShapeUpdated_Implementation()
{
	AActor* Owner = GetOwner();

	if (!Owner)
	{
		UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: Owner is null. Cannot update shape.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (InteractableComponents.Num() == 0 || !InteractableComponents[0])
	{
		UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: No interactable box component to update (InteractableComponents empty or null).",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	FBoxSphereBounds Bounds = FBoxSphereBounds();
	if (Owner->GetClass()->ImplementsInterface(USceneComponentCatchInterface::StaticClass()))
	{
		USceneComponent* Component = ISceneComponentCatchInterface::Execute_GetSceneComponent(Owner, FName("Mesh"));

		if (!Component)
		{
			UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: SceneComponentCatchInterface returned null for Mesh component.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		UMeshComponent* MeshComponent = Cast<UMeshComponent>(Component);
		if (!MeshComponent)
		{
			UE_LOGFMT(Log_Interact_Setup, Error, "{Time}: {Line} {Class}: SceneComponentCatchInterface returned a non-mesh component.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		Bounds = MeshComponent->GetLocalBounds();
	}
	else
	{
		UE_LOGFMT(Log_Interact_Setup, Warning, "{Time}: {Line} {Class}: Owner {OwnerName} does not implement SceneComponentCatchInterface. Keeping current box extent.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("OwnerName", *Owner->GetName()));
	}

	InteractableComponents[0]->SetBoxExtent(Bounds.BoxExtent, false);
	InteractableComponents[0]->SetRelativeLocation(FVector(0.f, 0.f, Bounds.BoxExtent.Z));
}

TArray<FInformationSet> UInteractCollisionComponent::GetSettingsVariableSets_Implementation() const
{
	TArray<FInformationSet> InformationSets = TArray<FInformationSet>();

	FInformationSet InteractInformationSet = UCoreInformationFunctionLibrary::GetInformationSet(
		"Interact",
		"",
		""
	);

	UCoreInformationFunctionLibrary::AddInformationValue(
		InteractInformationSet,
		"Has Limit",
		bHasLimit ? "True" : "False",
		EValueType::VT_BOOL
	);

	if(bHasLimit)
	{
		UCoreInformationFunctionLibrary::AddInformationValue(
			InteractInformationSet,
			"Limit",
			FString::FromInt(Limit),
			EValueType::VT_INT
		);
	}

	InformationSets.Add(InteractInformationSet);

	FInformationSet CurrentInteractingActorInformationSet = UCoreInformationFunctionLibrary::GetInformationSet(
		"Current Interacting Actor",
		"",
		""
	);
	if (CurrentInteractingActor)
	{
		UCoreInformationFunctionLibrary::AddInformationValue(
			CurrentInteractingActorInformationSet,
			"",
			CurrentInteractingActor->GetName(),
			EValueType::VT_STRING
		);
	}
	else
	{
		UCoreInformationFunctionLibrary::AddInformationValue(
			CurrentInteractingActorInformationSet,
			"",
			"No actor is currently interacting",
			EValueType::VT_STRING
		);
	}

	InformationSets.Add(CurrentInteractingActorInformationSet);

	FInformationSet InteractingActorsInformationSet = UCoreInformationFunctionLibrary::GetInformationSet(
		"Interactable Actors",
		"",
		""
	);

	if(InteractableActors.Num() == 0)
	{
		UCoreInformationFunctionLibrary::AddInformationValue(
			InteractingActorsInformationSet,
			"",
			"No actors found",
			EValueType::VT_STRING
		);
	}
	else
	{
		for (const AActor* Actor : InteractableActors)
		{
			if (Actor)
			{
				UCoreInformationFunctionLibrary::AddInformationValue(
					InteractingActorsInformationSet,
					"",
					Actor->GetName(),
					EValueType::VT_STRING
				);
			}
		}
	}

	InformationSets.Add(InteractingActorsInformationSet);

	InformationSets.Append(Super::GetSettingsVariableSets_Implementation());

	return InformationSets;
}
