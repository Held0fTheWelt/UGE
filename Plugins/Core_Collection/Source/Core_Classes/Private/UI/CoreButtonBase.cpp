// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "UI/CoreButtonBase.h"
#include "CommonActionWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreButtonBase)

void UCoreButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateButtonStyle();
	RefreshButtonText();
}

void UCoreButtonBase::UpdateInputActionWidget()
{
	Super::UpdateInputActionWidget();

	UpdateButtonStyle();
	RefreshButtonText();
}

void UCoreButtonBase::SetButtonText(const FText& InText)
{
	bOverride_ButtonText = InText.IsEmpty();
	ButtonText = InText;
	RefreshButtonText();
}

void UCoreButtonBase::RefreshButtonText()
{
	if (bOverride_ButtonText || ButtonText.IsEmpty())
	{
		if (InputActionWidget)
		{
			const FText ActionDisplayText = InputActionWidget->GetDisplayText();	
			if (!ActionDisplayText.IsEmpty())
			{
				UpdateButtonText(ActionDisplayText);
				return;
			}
		}
	}
	
	UpdateButtonText(ButtonText);	
}

void UCoreButtonBase::OnInputMethodChanged(ECommonInputType CurrentInputType)
{
	Super::OnInputMethodChanged(CurrentInputType);

	UpdateButtonStyle();
}
