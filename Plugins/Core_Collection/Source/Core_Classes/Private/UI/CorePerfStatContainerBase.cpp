// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "UI/CorePerfStatContainerBase.h"

#include "Blueprint/WidgetTree.h"
#include "UI/CorePerfStatWidgetBase.h"
#include "Settings/CoreSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CorePerfStatContainerBase)

//////////////////////////////////////////////////////////////////////
// UCorePerfStatContainerBase

UCorePerfStatContainerBase::UCorePerfStatContainerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCorePerfStatContainerBase::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateVisibilityOfChildren();

	UCoreSettingsLocal::Get()->OnPerfStatDisplayStateChanged().AddUObject(this, &ThisClass::UpdateVisibilityOfChildren);
}

void UCorePerfStatContainerBase::NativeDestruct()
{
	UCoreSettingsLocal::Get()->OnPerfStatDisplayStateChanged().RemoveAll(this);

	Super::NativeDestruct();
}

void UCorePerfStatContainerBase::UpdateVisibilityOfChildren()
{
	UCoreSettingsLocal* UserSettings = UCoreSettingsLocal::Get();

	const bool bShowTextWidgets = (StatDisplayModeFilter == ECoreStatDisplayMode::TextOnly) || (StatDisplayModeFilter == ECoreStatDisplayMode::TextAndGraph);
	const bool bShowGraphWidgets = (StatDisplayModeFilter == ECoreStatDisplayMode::GraphOnly) || (StatDisplayModeFilter == ECoreStatDisplayMode::TextAndGraph);
	
	check(WidgetTree);
	WidgetTree->ForEachWidget([&](UWidget* Widget)
	{
		if (UCorePerfStatWidgetBase* TypedWidget = Cast<UCorePerfStatWidgetBase>(Widget))
		{
			const ECoreStatDisplayMode SettingMode = UserSettings->GetPerfStatDisplayState(TypedWidget->GetStatToDisplay());

			bool bShowWidget = false;
			switch (SettingMode)
			{
			case ECoreStatDisplayMode::Hidden:
				bShowWidget = false;
				break;
			case ECoreStatDisplayMode::TextOnly:
				bShowWidget = bShowTextWidgets;
				break;
			case ECoreStatDisplayMode::GraphOnly:
				bShowWidget = bShowGraphWidgets;
				break;
			case ECoreStatDisplayMode::TextAndGraph:
				bShowWidget = bShowTextWidgets || bShowGraphWidgets;
				break;
			}

			TypedWidget->SetVisibility(bShowWidget ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
	});
}
