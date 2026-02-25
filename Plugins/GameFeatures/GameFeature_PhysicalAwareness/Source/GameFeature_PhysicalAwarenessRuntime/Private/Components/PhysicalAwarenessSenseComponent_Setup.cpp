// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PhysicalAwarenessSenseComponent.h"

#include "Components/PrimitiveComponent.h"

#include "DataAssets/PhysicalAwarenessSenseDataAsset.h"

#include "GameFramework/Actor.h"

#include "Interfaces/SceneComponentCatchInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/Logging_GF_PhysicalAwarenessSense.h"
#include "Logging/StructuredLog.h"

#include "Structs/InformationSet.h"

void UPhysicalAwarenessSenseComponent::SetupLoadedAsset(UObject* InOwner, UObject* LoadedObject)
{
	if (IsGameFeatureIgnored())
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Verbose, "{Time}: {Line} {Class}: Game feature is ignored, skipping SetupLoadedAsset on Component {ComponentName}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("ComponentName", GetNameSafe(this))
		);
		return;
	}

	if (!InOwner)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Error, "{Time}: {Line} {Class}: Owner is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if(!LoadedObject)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Error, "{Time}: {Line} {Class}: Loaded object is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (Cast<UActorComponent>(InOwner) != this)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, VeryVerbose, "{Time}: {Line} {Class}: Owner {Owner} is not this component. LoadedObjectName {Name}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", InOwner->GetFName()),
			("Name", LoadedObject->GetFName() ));
		return;
	}

	if (!LoadedObject->GetClass()->ImplementsInterface(UAssetTypeInterface::StaticClass()))
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Error, "{Time}: {Line} {Class}: Loaded object does not implement UAssetTypeInterface.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	FPrimaryAssetType PrimaryAssetId = IAssetTypeInterface::Execute_GetAssetType(LoadedObject);

	if (PrimaryAssetId == UPhysicalAwarenessSenseDataAsset::GetAssetType())
	{
		if (!LoadedObject)
		{
			UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Error, "{Time}: {Line} {Class}: Loaded object is null",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		UPhysicalAwarenessSenseDataAsset* Asset = Cast<UPhysicalAwarenessSenseDataAsset>(LoadedObject);

		if(!Asset)
		{
			UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Error, "{Time}: {Line} {Class}: Failed to cast loaded object to UPhysicalAwarenessSensePrimaryDataAsset",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			return;
		}

		bDoesTrackActorsForGameManagement = Asset->bDoesTrackActorsForGameManagement;
		bCanSenseActors = Asset->bCanSenseActors;
		bCanTouchActors = Asset->bCanTouchActors;
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Verbose, "{Time}: {Line} {Class}: Loaded PhysicalAwarenessSensePrimaryDataAsset: bDoesTrackActorsForGameManagement={bDoesTrackActorsForGameManagement}, bCanSenseActors={bCanSenseActors}, bCanTouchActors={bCanTouchActors}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("bDoesTrackActorsForGameManagement", bDoesTrackActorsForGameManagement),
			("bCanSenseActors", bCanSenseActors),
			("bCanTouchActors", bCanTouchActors));

		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Verbose, "{Time}: {Line} {Class}: PhysicalAwarenessSenseComponent set up and ready",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));

		if(!bDoesTrackActorsForGameManagement)
		{
			UE_LOGFMT(Log_PhysicalSenseComponent_Setup, VeryVerbose, "{Time}: {Line} {Class}: Tracking actors for game management is disabled. No actors will be registered or forgotten.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));

			SetCollisionComponentActive(false);

			// Clear the TouchableActors array since we won't be tracking actors
			TouchableActors.Empty();

			UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Warning, "{Time}: {Line} {Class}: Touchable Actors array cleared as tracking actors is disabled.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
		}
		else
		{
			UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Warning, "{Time}: {Line} {Class}: Tracking actors for game management is enabled. Actors will be registered and forgotten as they are sensed and unsensed.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
		}
	}
	else
	{
		//UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Warning, "{Time}: {Line} {Class}: Loaded asset is not a PhysicalAwarenessSensePrimaryDataAsset, passing to Super. LoadedObjectName: {Name}",
		//	("Class", GET_CLASSNAME_WITH_FUNCTION),
		//	("Line", GET_LINE_NUMBER),
		//	("Time", GET_CURRENT_TIME),
		//	("Name", LoadedObject->GetFName()));
	}

	Super::SetupLoadedAsset(InOwner, LoadedObject);
}

void UPhysicalAwarenessSenseComponent::SetCollisionComponentActive(bool bActive)
{
	UPrimitiveComponent* CollisionComponent = GetCollisionComponent();

	if (!CollisionComponent)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Error, "{Time}: {Line} {Class}: Collision component is not a valid UPrimitiveComponent. Cannot initialize PhysicalAwarenessSenseComponent.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
	else
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Verbose, "{Time}: {Line} {Class}: Collision component found: {CollisionComponentName}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("CollisionComponentName", *CollisionComponent->GetName()));
	}

	if (bActive)
	{
		CollisionComponent->BodyInstance.UpdatePhysicsFilterData();

		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &UPhysicalAwarenessSenseComponent::OnBeginOverlap);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &UPhysicalAwarenessSenseComponent::OnEndOverlap);
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Verbose, "{Time}: {Line} {Class}: OnComponentBeginOverlap and OnComponentEndOverlap events bound to OwningActor: {ActorName}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("ActorName", *PawnOwner->GetName()));
	}
	else
	{
		if (CollisionComponent->OnComponentBeginOverlap.IsBound())
		{
			CollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &UPhysicalAwarenessSenseComponent::OnBeginOverlap);
			UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Verbose, "{Time}: {Line} {Class}: OnComponentBeginOverlap and OnComponentEndOverlap events unbound from OwningActor: {ActorName}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("ActorName", *PawnOwner->GetName()));
		}
		if (CollisionComponent->OnComponentEndOverlap.IsBound())
		{
			CollisionComponent->OnComponentEndOverlap.RemoveDynamic(this, &UPhysicalAwarenessSenseComponent::OnEndOverlap);
			UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Verbose, "{Time}: {Line} {Class}: OnComponentBeginOverlap and OnComponentEndOverlap events unbound from OwningActor: {ActorName}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("ActorName", *PawnOwner->GetName()));
		}

	}
}

UPrimitiveComponent* UPhysicalAwarenessSenseComponent::GetCollisionComponent() const
{
	if (!IsPawnOwnerValid())
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Error, "{Time}: {Line} {Class}: Owning actor is null. Cannot initialize PhysicalAwarenessSenseComponent.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return nullptr;
	}
	USceneComponent* SceneComponent = nullptr;

	if (PawnOwner->GetClass()->ImplementsInterface(USceneComponentCatchInterface::StaticClass()))
	{
		SceneComponent = ISceneComponentCatchInterface::Execute_GetSceneComponent(PawnOwner, FName("Collision"));
	}
	else
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Error, "{Time}: {Line} {Class}: Owning actor {ActorName} does not implement SceneComponentCatchInterface. Cannot initialize PhysicalAwarenessSenseComponent.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("ActorName", *PawnOwner->GetName()));
	}

	if (!SceneComponent)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Error, "{Time}: {Line} {Class}: Collision component is null. Cannot initialize PhysicalAwarenessSenseComponent.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return nullptr;
	}

	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(SceneComponent);
	if (!PrimitiveComponent)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Error, "{Time}: {Line} {Class}: Scene component \"Collision\" is not a UPrimitiveComponent (got {ClassName}). Cannot initialize PhysicalAwarenessSenseComponent.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("ClassName", *SceneComponent->GetClass()->GetName()));
		return nullptr;
	}
	return PrimitiveComponent;
}

TArray<FInformationSet> UPhysicalAwarenessSenseComponent::GetSettingsVariableSets_Implementation() const
{
	if (!IsPawnOwnerValid())
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Error, "{Time}: {Line} {Class}: Owning actor is null. Cannot get settings variable sets.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return TArray<FInformationSet>();
	}

	TArray<FInformationSet> InformationSets;

	// Add the basic information set for the component
	FInformationSet BasicInformationSet;
	BasicInformationSet.InformationCategory= "Sense Information";
	//BasicInformationSet.InformationName = "Physical Awareness Sense Component Settings";
	BasicInformationSet.InformationDescription = "Settings for the Physical Awareness Sense Component";

	FInformationSetInformation TrackInformation;

	TrackInformation.VisibleName = "DoesTrackActors";
	TrackInformation.Value = bDoesTrackActorsForGameManagement ? "True" : "False";

	BasicInformationSet.InformationValues.Add(TrackInformation);

	FInformationSetInformation SenseInformation;
	SenseInformation.VisibleName = "CanSenseActors";
	SenseInformation.Value = bCanSenseActors ? "True" : "False";

	BasicInformationSet.InformationValues.Add(SenseInformation);

	FInformationSetInformation TouchInformation;
	TouchInformation.VisibleName = "CanTouchActors";
	TouchInformation.Value = bCanTouchActors ? "True" : "False";

	BasicInformationSet.InformationValues.Add(TouchInformation);

	InformationSets.Add(BasicInformationSet);

	InformationSets.Append(Super::GetSettingsVariableSets_Implementation());

	UE_LOGFMT(Log_PhysicalSenseComponent_Setup, Verbose, "{Time}: {Line} {Class}: Returning settings variable sets with {NumSets} sets.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("NumSets", InformationSets.Num()));

	return InformationSets;
}
