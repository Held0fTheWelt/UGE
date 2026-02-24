// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GameplayPadActor.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"

AGameplayPadActor::AGameplayPadActor()
{
	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	Volume->SetupAttachment(RootComponent);
	
	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(RootComponent);
	
	PadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadMesh"));
	PadMesh->SetupAttachment(RootComponent);
	
	PadBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadBase"));
	PadBase->SetupAttachment(RootComponent);
}
