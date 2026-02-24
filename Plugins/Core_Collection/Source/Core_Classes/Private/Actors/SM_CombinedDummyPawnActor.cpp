// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SM_CombinedDummyPawnActor.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"


ASM_CombinedDummyPawnActor::ASM_CombinedDummyPawnActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
}

void ASM_CombinedDummyPawnActor::BeginPlay()
{
	Super::BeginPlay();
}

#pragma region SceneComponentCatchInterface
USceneComponent* ASM_CombinedDummyPawnActor::GetSceneComponent_Implementation(const FName& ComponentName) const
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