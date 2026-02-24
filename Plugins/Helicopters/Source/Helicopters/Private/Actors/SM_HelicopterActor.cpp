// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SM_HelicopterActor.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Logging/GlobalLog.h"
#include "Logging/Logging_Helicopters.h"
#include "Logging/StructuredLog.h"

ASM_HelicopterActor::ASM_HelicopterActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HelicopterMesh"));
	RootComponent = Mesh;

	Forward->SetupAttachment(Mesh);
}

USceneComponent* ASM_HelicopterActor::GetSceneComponent_Implementation(const FName& ComponentName) const
{
	if (ComponentName == TEXT("Mesh") || ComponentName == TEXT("Collision"))
	{
		return Mesh;
	}
	return Super::GetSceneComponent_Implementation(ComponentName);
}