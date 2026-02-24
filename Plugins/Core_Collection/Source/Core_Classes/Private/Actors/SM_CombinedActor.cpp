// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SM_CombinedActor.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"


ASM_CombinedActor::ASM_CombinedActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
}


#pragma region SceneComponentCatchInterface
USceneComponent* ASM_CombinedActor::GetSceneComponent_Implementation(const FName& ComponentName) const
{
	if (ComponentName == FName("Forward") && Forward)
	{
		return Forward;
	}
	else if (ComponentName == FName("Mesh") && Mesh)
	{
		return Mesh;
	}
	return Super::GetSceneComponent_Implementation(ComponentName);
}
#pragma endregion SceneComponentCatchInterface