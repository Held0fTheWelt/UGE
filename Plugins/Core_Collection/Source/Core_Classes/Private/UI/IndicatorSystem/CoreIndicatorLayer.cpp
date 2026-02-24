// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "UI/IndicatorSystem/CoreIndicatorLayer.h"

#include "UI/IndicatorSystem/SActorCanvas.h"
#include "Widgets/Layout/SBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreIndicatorLayer)

class SWidget;

/////////////////////////////////////////////////////
// UCoreIndicatorLayer

UCoreIndicatorLayer::UCoreIndicatorLayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = true;
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UCoreIndicatorLayer::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyActorCanvas.Reset();
}

TSharedRef<SWidget> UCoreIndicatorLayer::RebuildWidget()
{
	if (!IsDesignTime())
	{
		ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
		if (ensureMsgf(LocalPlayer, TEXT("Attempting to rebuild a UActorCanvas without a valid LocalPlayer!")))
		{
			MyActorCanvas = SNew(SActorCanvas, FLocalPlayerContext(LocalPlayer), &ArrowBrush);
			return MyActorCanvas.ToSharedRef();
		}
	}

	// Give it a trivial box, NullWidget isn't safe to use from a UWidget
	return SNew(SBox);
}
