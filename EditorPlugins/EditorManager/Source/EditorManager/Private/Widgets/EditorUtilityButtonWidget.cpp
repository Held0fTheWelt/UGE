// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/EditorUtilityButtonWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UEditorUtilityButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UEditorUtilityButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UEditorUtilityButtonWidget::OnButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EditorUtilityButtonWidget: Button is null! Please check your widget setup."));
	}
}

void UEditorUtilityButtonWidget::SetText(const FText& InText)
{
	if (Text)
	{
		Text->SetText(InText);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EditorUtilityButtonWidget: Text block is null! Please check your widget setup."));
	}
}

void UEditorUtilityButtonWidget::SetSelected(bool bSelected)
{
	if (Button)
	{
		FButtonStyle Style = Button->GetStyle();
		FSlateBrush NormalBrush = Style.Normal;
		NormalBrush.TintColor = bSelected ? FSlateColor(ButtonSelectedColor) : FSlateColor(ButtonUnselectedColor);
		Style.SetNormal(NormalBrush);
		Button->SetStyle(Style);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EditorUtilityButtonWidget: Button is null! Please check your widget setup."));
	}
}

void UEditorUtilityButtonWidget::OnButtonClicked()
{
	if (OnButtonClickedDelegate.IsBound())
	{
		OnButtonClickedDelegate.Broadcast(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EditorUtilityButtonWidget: ButtonClicked delegate is not bound!"));
	}
}