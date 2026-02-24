// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SKM_CombinedDummyPawnActor.h"

#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"


ASKM_CombinedDummyPawnActor::ASKM_CombinedDummyPawnActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void ASKM_CombinedDummyPawnActor::BeginPlay()
{
	Super::BeginPlay();
}

#pragma region SceneComponentCatchInterface
USceneComponent* ASKM_CombinedDummyPawnActor::GetSceneComponent_Implementation(const FName& ComponentName) const
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