// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/DummyActor.h"

#include "Components/ArrowComponent.h"

ADummyActor::ADummyActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	IgnoredGameFeatures = TArray<FString>();
	DataAssetIdentifier = FName(TEXT("BaseActorDefinition"));
	PrimaryAssetNamesToLoad = { FName(TEXT("BaseActorDefinition")) };

	Forward->SetupAttachment(Root);
}