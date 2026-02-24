// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/SM_AircraftActor.h"

#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"

ASM_AircraftActor::ASM_AircraftActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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

USceneComponent* ASM_AircraftActor::GetSceneComponent_Implementation(const FName& ComponentName) const
{
	if(ComponentName == TEXT("Mesh"))
	{
		return Mesh;
	}

	else if (ComponentName == TEXT("EngineSound"))
	{
		return EngineSound;
	}

	return Super::GetSceneComponent(ComponentName);
}
