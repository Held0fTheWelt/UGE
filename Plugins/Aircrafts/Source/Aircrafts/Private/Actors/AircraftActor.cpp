// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AircraftActor.h"

#include "Components/SceneComponent.h"
#include "Components/AudioComponent.h"

#include "GameFramework/FloatingPawnMovement.h"

AAircraftActor::AAircraftActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	EngineSound = CreateDefaultSubobject<UAudioComponent>(TEXT("PlaneEngineSound"));
	EngineSound->SetupAttachment(Root);
	EngineSound->bAutoActivate = false; // Set to false to control playback manually
	EngineSound->bAllowSpatialization = true; // Enable spatialization for 3D sound

	AircraftMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("AircraftMovementComponent"));
	//AircraftMovementComponent->UpdatedComponent = Root;
	//AircraftMovementComponent->bConstrainToPlane = true; // Constrain movement to a plane
	//AircraftMovementComponent->bSnapToPlaneAtStart = true; // Snap to plane at start
	//AircraftMovementComponent->bUseAccelerationForPaths = true; // Use acceleration for movement paths

}

void AAircraftActor::SetSpeed_Implementation(float Speed)
{
	if(AircraftMovementComponent)
	{
		AircraftMovementComponent->MaxSpeed = Speed;
		AircraftMovementComponent->Velocity = AircraftMovementComponent->MaxSpeed * GetActorForwardVector();
		UE_LOG(LogTemp, Log, TEXT("Aircraft speed set to: %f"), Speed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AircraftMovementComponent is not initialized."));
	}
}
