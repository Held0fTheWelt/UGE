// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "UI/Settings/CoreSafeZoneEditor.h"

#include "CommonButtonBase.h"
#include "CommonRichTextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "GameSettingValueScalar.h"
#include "Settings/CoreSettingsLocal.h"
#include "Widgets/Layout/SSafeZone.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreSafeZoneEditor)

struct FGeometry;

#define LOCTEXT_NAMESPACE "Core"

namespace SafeZoneEditor
{
	const float JoystickDeadZone = 0.2f;
	const float SafeZoneChangeSpeed = 0.1f;	
}

UCoreSafeZoneEditor::UCoreSafeZoneEditor(const FObjectInitializer& Initializer)
	: Super(Initializer)
{
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);
}

void UCoreSafeZoneEditor::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Switcher_SafeZoneMessage->SetActiveWidget(RichText_Default);
}

void UCoreSafeZoneEditor::NativeOnActivated()
{
	Super::NativeOnActivated();

	SSafeZone::SetGlobalSafeZoneScale(UCoreSettingsLocal::Get()->GetSafeZone());
	
	Button_Done->OnClicked().AddUObject(this, &UCoreSafeZoneEditor::HandleDoneClicked);

	Button_Back->SetVisibility((bCanCancel)? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (bCanCancel)
	{
		Button_Back->OnClicked().AddUObject(this, &UCoreSafeZoneEditor::HandleBackClicked);
	}
}

bool UCoreSafeZoneEditor::ExecuteActionForSetting_Implementation(FGameplayTag ActionTag, UGameSetting* InSetting)
{
	TArray<UGameSetting*> ChildSettings = InSetting ? InSetting->GetChildSettings() : TArray<UGameSetting*>();
	if (ChildSettings.Num() > 0 && ChildSettings[0])
	{
		ValueSetting = Cast<UGameSettingValueScalar>(ChildSettings[0]);
	}

	return true;
}

FReply UCoreSafeZoneEditor::NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent)
{
	if (InAnalogEvent.GetKey() == EKeys::Gamepad_LeftY && FMath::Abs(InAnalogEvent.GetAnalogValue()) >= SafeZoneEditor::JoystickDeadZone)
	{
		const float SafeZoneMultiplier = FMath::Clamp(SSafeZone::GetGlobalSafeZoneScale().Get(1.0f) + InAnalogEvent.GetAnalogValue() * SafeZoneEditor::SafeZoneChangeSpeed, 0.0f, 1.0f);
		SSafeZone::SetGlobalSafeZoneScale(SafeZoneMultiplier >= 0 ? SafeZoneMultiplier : 0);
		
		return FReply::Handled();
	}
	return Super::NativeOnAnalogValueChanged(InGeometry, InAnalogEvent);
}

FReply UCoreSafeZoneEditor::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const float SafeZoneMultiplier = FMath::Clamp(SSafeZone::GetGlobalSafeZoneScale().Get(1.0f) + InMouseEvent.GetWheelDelta() * SafeZoneEditor::SafeZoneChangeSpeed, 0.0f, 1.0f);
	SSafeZone::SetGlobalSafeZoneScale(SafeZoneMultiplier >= 0 ? SafeZoneMultiplier : 0);

	return FReply::Handled();
}

void UCoreSafeZoneEditor::HandleInputModeChanged(ECommonInputType InInputType)
{
	const FText KeyName = InInputType == ECommonInputType::Gamepad ? LOCTEXT("SafeZone_KeyToPress_Gamepad", "Left Stick") : LOCTEXT("SafeZone_KeyToPress_Mouse", "Mouse Wheel");
	RichText_Default->SetText(FText::Format(LOCTEXT("SafeZoneEditorInstructions", "Use <text color=\"FFFFFFFF\" fontface=\"black\">{0}</> to adjust the corners so it lines up with the edges of your display."), KeyName));
}

void UCoreSafeZoneEditor::HandleBackClicked()
{
	DeactivateWidget();
	SSafeZone::SetGlobalSafeZoneScale(UCoreSettingsLocal::Get()->GetSafeZone());
}

void UCoreSafeZoneEditor::HandleDoneClicked()
{
	if (ValueSetting.IsValid())
	{
		ValueSetting.Get()->SetValue(SSafeZone::GetGlobalSafeZoneScale().Get(1.0f));
	}
	else
	{
		UCoreSettingsLocal::Get()->SetSafeZone(SSafeZone::GetGlobalSafeZoneScale().Get(1.0f));
	}
	OnSafeZoneSet.Broadcast();
	DeactivateWidget();
}

#undef LOCTEXT_NAMESPACE
