// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SKM_CombinedActor.h"

#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"


ASKM_CombinedActor::ASKM_CombinedActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

#pragma region SceneComponentCatchInterface
USceneComponent* ASKM_CombinedActor::GetSceneComponent_Implementation(const FName& ComponentName) const
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