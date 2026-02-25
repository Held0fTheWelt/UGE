// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteractUserWidgetBase.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInteractUserWidgetBase::SetImage_Implementation(UTexture2D* InTexture)
{
	if (InTexture)
	{
		Image->SetBrushFromTexture(InTexture, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Image is not bound in InteractUserWidgetBase!"));
	}
}

void UInteractUserWidgetBase::SetText_Implementation(const FString& String)
{
	if (Text)
	{
		Text->SetText(FText::FromString(String));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Text block is not bound in InteractUserWidgetBase!"));
	}
}
