// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Subsystem/CoreLoadingScreenSubsystem.h"

#include "Blueprint/UserWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreLoadingScreenSubsystem)

class UUserWidget;

//////////////////////////////////////////////////////////////////////
// UCoreLoadingScreenSubsystem

UCoreLoadingScreenSubsystem::UCoreLoadingScreenSubsystem()
{
}

void UCoreLoadingScreenSubsystem::SetLoadingScreenContentWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (LoadingScreenWidgetClass != NewWidgetClass)
	{
		LoadingScreenWidgetClass = NewWidgetClass;

		OnLoadingScreenWidgetChanged.Broadcast(LoadingScreenWidgetClass);
	}
}

TSubclassOf<UUserWidget> UCoreLoadingScreenSubsystem::GetLoadingScreenContentWidget() const
{
	return LoadingScreenWidgetClass;
}
