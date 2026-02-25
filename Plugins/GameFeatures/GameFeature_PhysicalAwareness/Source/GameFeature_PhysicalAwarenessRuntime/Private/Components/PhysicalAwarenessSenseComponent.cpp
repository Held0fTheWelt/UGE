// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PhysicalAwarenessSenseComponent.h"

#include "Components/PrimitiveComponent.h"

#include "GameFramework/Actor.h"

#include "Interfaces/SceneComponentCatchInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/Logging_GF_PhysicalAwarenessSense.h"
#include "Logging/StructuredLog.h"


UPhysicalAwarenessSenseComponent::UPhysicalAwarenessSenseComponent(const FObjectInitializer& ObjectInitializer)
{
	bDoNotRegisterOnPossessControl = true; // Default value, can be set in derived classes or at runtime
	GameFeatureName = "PhysicalAwareness";
	bComponentHasComponentAssetToLoad = false;
}

void UPhysicalAwarenessSenseComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOGFMT(Log_PhysicalSenseComponent, VeryVerbose, "{Time}: {Line} {Class}: Initializing PhysicalAwarenessSenseComponent for GameFeature: {GameFeatureName}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("GameFeatureName", GameFeatureName));

	if (IsGameFeatureIgnored())
	{
		UE_LOGFMT(Log_PhysicalSenseComponent, Verbose, "{Time}: {Line} {Class}: Game feature is ignored, skipping PhysicalAwarenessSenseComponent init for {GameFeatureName}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("GameFeatureName", GameFeatureName));
		return;
	}

	TouchableActors.Empty(); // Clear the array of touchable actors
	UE_LOGFMT(Log_PhysicalSenseComponent, Verbose, "{Time}: {Line} {Class}: PhysicalAwarenessSenseComponent initialized for GameFeature: {GameFeatureName}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("GameFeatureName", GameFeatureName));

	SetCollisionComponentActive(true);
}

void UPhysicalAwarenessSenseComponent::RegisterObject(AActor* ActorToRegister, ESenseType Type)
{
	switch (Type)
	{
	case ESenseType::ST_TOUCH:
		if (ActorToRegister && !TouchableActors.Contains(ActorToRegister))
		{
			TouchableActors.Add(ActorToRegister);
			UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, Verbose, "{Time}: {Line} {Class}: Registered Actor {Actor} for touch sensing.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Actor", *ActorToRegister->GetName()));
		}
		break;
	default:
		break;
	}
}

void UPhysicalAwarenessSenseComponent::ForgetObject(AActor* ActorToForget, ESenseType Type)
{
	switch (Type)
	{
	case ESenseType::ST_TOUCH:
		if (ActorToForget && TouchableActors.Contains(ActorToForget))
		{
			TouchableActors.Remove(ActorToForget);
			UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, Verbose, "{Time}: {Line} {Class}: Forgotten Actor {Actor} from touch sensing.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Actor", *ActorToForget->GetName()));
		}
		break;
	default:
		break;
	}
}

void UPhysicalAwarenessSenseComponent::SetTouchingCurrentActor_Implementation(const AActor* Actor)
{
	// Check if the actor is valid before setting it as the current touched actor
	if (!Actor)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent, Verbose, "{Time}: {Line} {Class}: Setting current touched actor to a null actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		CurrentTouchedActor = nullptr; // Set to null if the actor is invalid
		return;
	}
	CurrentTouchedActor = const_cast<AActor*>(Actor); // Cast away constness to allow setting
#if WITH_EDITOR
	if (CurrentTouchedActor)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent, Verbose, "{Time}: {Line} {Class}: Set current touched actor to {Actor}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Actor", *CurrentTouchedActor->GetName()));
	}
	else
	{
		UE_LOGFMT(Log_PhysicalSenseComponent, Verbose, "{Time}: {Line} {Class}: Current touched actor is now null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
#endif
}

bool UPhysicalAwarenessSenseComponent::CanTouchCurrentActor_Implementation() const
{
	// Check if there are any touchable actors registered
	UE_LOGFMT(Log_PhysicalSenseComponent, Verbose, "{Time}: {Line} {Class}: Current actor can be touched: {Result}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Result", TouchableActors.Num() > 0));

	if (TouchableActors.Num() > 0)
	{
		// Assuming the first actor in the list is the current touchable actor
		return true;
	}
	return false;
}

AActor* UPhysicalAwarenessSenseComponent::GetCurrentTouchActor_Implementation() const
{
	// Check if there are any touchable actors registered
	UE_LOGFMT(Log_PhysicalSenseComponent, Verbose, "{Time}: {Line} {Class}: Getting current touch actor",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	if (TouchableActors.Num() > 0)
	{
		// Assuming the first actor in the list is the current touchable actor
		return TouchableActors[0].Get();
	}
	UE_LOGFMT(Log_PhysicalSenseComponent, Verbose, "{Time}: {Line} {Class}: No touchable actors found.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	return nullptr;
}

bool UPhysicalAwarenessSenseComponent::IsTouchingCurrentActor_Implementation() const
{
	// Check if the current touched actor is valid
	UE_LOGFMT(Log_PhysicalSenseComponent, VeryVerbose, "{Time}: {Line} {Class}: Checking if touching current actor: {Result}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Result", CurrentTouchedActor != nullptr));
	return CurrentTouchedActor != nullptr;
}

bool UPhysicalAwarenessSenseComponent::CanTouchActor_Implementation(const AActor* Actor) const
{
	if (!Actor)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, Warning, "{Time}: {Line} {Class}: Cannot check touchability for a null actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return false;
	}
	// Check if the actor is in the list of touchable actors
	UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, Verbose, "{Time}: {Line} {Class}: Actor {Actor} can be touched: {Result}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Actor", *Actor->GetName()),
		("Result", TouchableActors.Contains(Actor)));

	if (TouchableActors.Contains(Actor))
	{
		// If the actor is in the list, it can be touched
		UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, VeryVerbose, "{Time}: {Line} {Class}: Actor {Actor} is touchable.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Actor", *Actor->GetName()));
		return true;
	}
	// If the actor is not in the list, it cannot be touched
	UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, VeryVerbose, "{Time}: {Line} {Class}: Actor {Actor} is not touchable.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Actor", *Actor->GetName()));
	return false;
}

TArray<TSoftObjectPtr<AActor>> UPhysicalAwarenessSenseComponent::GetTouchableActors_Implementation() const
{
	UE_LOGFMT(Log_PhysicalSenseComponent, VeryVerbose, "{Time}: {Line} {Class}: Getting list of touchable actors",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME));
	return TouchableActors;
}