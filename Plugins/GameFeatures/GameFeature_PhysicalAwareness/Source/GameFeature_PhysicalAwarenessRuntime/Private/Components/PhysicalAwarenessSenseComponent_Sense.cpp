// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PhysicalAwarenessSenseComponent.h"

#include "Components/PrimitiveComponent.h"

#include "GameFramework/Actor.h"

#include "Interfaces/NoTouchCollisionInterface.h"
#include "Interfaces/SceneComponentCatchInterface.h"
#include "Interfaces/Control/InteractControlInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/Logging_GF_PhysicalAwarenessSense.h"
#include "Logging/StructuredLog.h"

void UPhysicalAwarenessSenseComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!bDoesTrackActorsForGameManagement)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, VeryVerbose, "{Time}: {Line} {Class}: Tracking actors for game management is disabled. Overlap event ignored.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if(!OverlappedComponent || !OtherActor || !OtherComp)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, Error, "{Time}: {Line} {Class}: Invalid parameters in OnBeginOverlap. OverlappedComponent: {OverlappedComponent}, OtherActor: {OtherActor}, OtherComp: {OtherComp}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("OverlappedComponent", OverlappedComponent ? *OverlappedComponent->GetName() : TEXT("None")),
			("OtherActor", OtherActor ? *OtherActor->GetName() : TEXT("None")),
			("OtherComp", OtherComp ? *OtherComp->GetName() : TEXT("None")));
		return;
	}

	if (Cast<INoTouchCollisionInterface>(OtherComp))
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, VeryVerbose, "{Time}: {Line} {Class}: OverlappedComponent {OverlappedComponentName} implements INoTouchCollisionInterface. Overlap event ignored.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("OverlappedComponentName", *OtherComp->GetName()));
		return;
	}

	SenseTouchActor(OtherActor, true);
	
	UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, VeryVerbose, "{Time}: {Line} {Class}: OnBeginOverlap triggered with OtherActor: {OtherActorName}, OverlappedComponent: {OverlappedComponentName}, OtherComp: {OtherCompName}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("OtherActorName", OtherActor ? *OtherActor->GetName() : TEXT("None")),
		("OverlappedComponentName", OverlappedComponent ? *OverlappedComponent->GetName() : TEXT("None")),
		("OtherCompName", OtherComp ? *OtherComp->GetName() : TEXT("None")));
}

void UPhysicalAwarenessSenseComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bDoesTrackActorsForGameManagement)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, VeryVerbose, "{Time}: {Line} {Class}: Tracking actors for game management is disabled. Overlap event ignored.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!OverlappedComponent || !OtherActor || !OtherComp)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, Error, "{Time}: {Line} {Class}: Invalid parameters in OnEndOverlap. OverlappedComponent: {OverlappedComponent}, OtherActor: {OtherActor}, OtherComp: {OtherComp}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("OverlappedComponent", OverlappedComponent ? *OverlappedComponent->GetName() : TEXT("None")),
			("OtherActor", OtherActor ? *OtherActor->GetName() : TEXT("None")),
			("OtherComp", OtherComp ? *OtherComp->GetName() : TEXT("None")));
		return;
	}

	if (Cast<INoTouchCollisionInterface>(OtherComp))
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, VeryVerbose, "{Time}: {Line} {Class}: OverlappedComponent {OverlappedComponentName} implements INoTouchCollisionInterface. Overlap event ignored.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("OverlappedComponentName", *OtherComp->GetName()));
		return;
	}

	SenseTouchActor(OtherActor, false);

	UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, VeryVerbose, "{Time}: {Line} {Class}: OnEndOverlap triggered with OtherActor: {OtherActorName}, OverlappedComponent: {OverlappedComponentName}, OtherComp: {OtherCompName}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("OtherActorName", OtherActor ? *OtherActor->GetName() : TEXT("None")),
		("OverlappedComponentName", OverlappedComponent ? *OverlappedComponent->GetName() : TEXT("None")),
		("OtherCompName", OtherComp ? *OtherComp->GetName() : TEXT("None")));
}

void UPhysicalAwarenessSenseComponent::SenseTouchActor(AActor* Actor, bool On)
{
	if (!Actor)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, Error, "{Time}: {Line} {Class}: SenseTouchActor called with null Actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	TArray<UActorComponent*> Components = Actor->GetComponentsByInterface(UInteractControlInterface::StaticClass());

	if(Components.Num() == 0)
	{
		UE_LOGFMT(Log_PhysicalSenseComponent_Runtime, VeryVerbose, "{Time}: {Line} {Class}: Actor {Actor} does not implement UInteractControllerInterface. Cannot register or forget object for touch sensing.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Actor", *Actor->GetName()));
		return;
	}

	if (On)
	{
		RegisterObject(Actor, ESenseType::ST_TOUCH);
	}
	else
	{
		ForgetObject(Actor, ESenseType::ST_TOUCH);
	}
}
