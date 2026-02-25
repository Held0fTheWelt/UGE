// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Demo/DemoDisplayActor.h"

#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"

ADemoDisplayActor::ADemoDisplayActor()
{
	PrimaryActorTick.bCanEverTick = false;

	if (Root == nullptr)
	{
		Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	}

	SetRootComponent(Root);

	if (TextRender == nullptr)
	{
		TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	}

	TextRender->SetupAttachment(RootComponent);


}

void ADemoDisplayActor::BeginPlay()
{
	Super::BeginPlay();
	//if (TextRender)
	//{
	//	TextRender->SetText(FText::FromString(TEXT("Demo Display Actor")));
	//	TextRender->SetTextRenderColor(FColor::White);
	//	TextRender->SetHorizontalAlignment(EHTA_Center);
	//	TextRender->SetVerticalAlignment(EVRTA_TextCenter);
	//	TextRender->SetWorldSize(100.0f);
	//}
	
	Color = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, TextRender->GetMaterial(0));

	OnDemoColorChanged.AddDynamic(this, &ADemoDisplayActor::UpdateDisplayColor);
}

void ADemoDisplayActor::UpdateDisplayColor(const FLinearColor& InColor)
{
}

