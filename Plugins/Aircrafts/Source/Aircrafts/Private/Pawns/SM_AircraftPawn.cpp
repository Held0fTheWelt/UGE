// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawns/SM_AircraftPawn.h"

#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

ASM_AircraftPawn::ASM_AircraftPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//Mesh->SetCollisionProfileName(TEXT("NoCollision")); // Set collision profile to NoCollision
	Mesh->bCastDynamicShadow = false; // Disable dynamic shadows
	Mesh->bAffectDistanceFieldLighting = false; // Disable distance field lighting
	Mesh->bReceivesDecals = false; // Disable decals
	Mesh->bUseAsOccluder = false; // Disable occlusion

	EngineSound->SetupAttachment(Mesh);
}

USceneComponent* ASM_AircraftPawn::GetSceneComponent_Implementation(const FName& ComponentName) const
{
	if (ComponentName == TEXT("Mesh"))
	{
		return Mesh;
	}

	else if (ComponentName == TEXT("EngineSound"))
	{
		return EngineSound;
	}
	else if (ComponentName == TEXT("Forward"))
	{
		return Forward;
	}

	return Super::GetSceneComponent(ComponentName);
}
