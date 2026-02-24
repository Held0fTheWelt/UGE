// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/ShipPawn.h"

//#include "Controllers/AircraftAutopilotAIController.h"

#include "Components/SceneComponent.h"
#include "Components/AudioComponent.h"

#include "GameFramework/FloatingPawnMovement.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(Log_ShipPawn);

AShipPawn::AShipPawn(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	EngineSound = CreateDefaultSubobject<UAudioComponent>(TEXT("PlaneEngineSound"));
	EngineSound->SetupAttachment(Root);
	EngineSound->bAutoActivate = false; // Set to false to control playback manually
	EngineSound->bAllowSpatialization = true; // Enable spatialization for 3D sound

	//AircraftMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("AircraftMovementComponent"));
	//AircraftMovementComponent->PrimaryComponentTick.bCanEverTick = true;
	//AircraftMovementComponent->SetComponentTickEnabled(true);
	//AircraftMovementComponent->SetUpdatedComponent(RootComponent);

	//AIControllerClass = AAircraftAutopilotAIController::StaticClass(); // Set the AI controller class for this pawn
}
//
//void AAircraftPawn::SetSpeed_Implementation(float Speed)
//{
//	if (AircraftMovementComponent)
//	{
//		AircraftMovementComponent->MaxSpeed = Speed; // Set the maximum speed of the aircraft
//		UE_LOGFMT(Log_ShipPawn, Verbose, "{Time}: {Line} {Class}: Aircraft speed set to: {Speed}",
//			("Class", GET_CLASSNAME_WITH_FUNCTION),
//			("Line", GET_LINE_NUMBER),
//			("Time", GET_CURRENT_TIME),
//			("Speed", Speed));
//	}
//	else
//	{
//		UE_LOGFMT(Log_ShipPawn, Warning, "{Time}: {Line} {Class}: AircraftMovementComponent is not initialized.",
//			("Class", GET_CLASSNAME_WITH_FUNCTION),
//			("Line", GET_LINE_NUMBER),
//			("Time", GET_CURRENT_TIME));
//	}
//}