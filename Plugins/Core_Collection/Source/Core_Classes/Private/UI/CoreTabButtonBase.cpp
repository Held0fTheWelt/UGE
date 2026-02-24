// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "UI/CoreTabButtonBase.h"

#include "CommonLazyImage.h"
#include "UI/CoreTabListWidgetBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreTabButtonBase)

class UObject;
struct FSlateBrush;

void UCoreTabButtonBase::SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrushFromLazyDisplayAsset(LazyObject);
	}
}

void UCoreTabButtonBase::SetIconBrush(const FSlateBrush& Brush)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrush(Brush);
	}
}

void UCoreTabButtonBase::SetTabLabelInfo_Implementation(const FCoreTabDescriptor& TabLabelInfo)
{
	SetButtonText(TabLabelInfo.TabText);
	SetIconBrush(TabLabelInfo.IconBrush);
}
