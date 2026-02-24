// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "UI/CoreTouchRegion.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreTouchRegion)

struct FGeometry;
struct FPointerEvent;

FReply UCoreTouchRegion::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	bShouldSimulateInput = true;
	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UCoreTouchRegion::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	bShouldSimulateInput = true;
	return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
}

FReply UCoreTouchRegion::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	bShouldSimulateInput = false;
	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
}

void UCoreTouchRegion::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(bShouldSimulateInput)
	{
		InputKeyValue(FVector::OneVector);
	}
}
