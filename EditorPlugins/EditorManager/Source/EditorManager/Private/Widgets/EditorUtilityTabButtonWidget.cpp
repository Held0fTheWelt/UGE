// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EditorUtilityTabButtonWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Engine/Texture2D.h"

UEditorUtilityTabButtonWidget::UEditorUtilityTabButtonWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UEditorUtilityTabButtonWidget::SetSelected(bool bSelected)
{
	if (TabButton)
	{
		FButtonStyle Style = TabButton->GetStyle();

		if (bSelected)
		{
			if(SelectedIcon)
			{
				//UE_LOG(LogTemp, Log, TEXT("Setting TabButton to selected with icon: %s"), *SelectedIcon->GetName());
				FSlateBrush Brush;
				Brush.SetResourceObject(SelectedIcon);
				Style.SetNormal(Brush);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("SelectedIcon is null!"));
			}
		}
		else 
		{
			if (UnselectedIcon)
			{
				//UE_LOG(LogTemp, Log, TEXT("Setting TabButton to unselected with icon: %s"), *UnselectedIcon->GetName());
				FSlateBrush Brush;
				Brush.SetResourceObject(UnselectedIcon);
				Style.SetNormal(Brush);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UnselectedIcon is null!"));
			}
		}

		TabButton->SetStyle(Style);
	}
}

void UEditorUtilityTabButtonWidget::SetTabName(const FText& Name)
{
	if (TabName)
	{
		TabName->SetText(Name);
	}
}

void UEditorUtilityTabButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UEditorUtilityTabButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TabName)
	{
		TabName->SetText(FText::FromString("Default Tab Name"));
	}
	if (TabButton)
	{
		TabButton->OnClicked.AddDynamic(this, &UEditorUtilityTabButtonWidget::OnTabButtonClicked);
	}
}

void UEditorUtilityTabButtonWidget::OnTabButtonClicked()
{
	OnTabButtonClickedDelegate.Broadcast(this);
}
