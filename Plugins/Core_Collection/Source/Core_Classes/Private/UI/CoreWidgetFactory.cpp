// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "UI/CoreWidgetFactory.h"
#include "Templates/SubclassOf.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreWidgetFactory)

class UUserWidget;

TSubclassOf<UUserWidget> UCoreWidgetFactory::FindWidgetClassForData_Implementation(const UObject* Data) const
{
	return TSubclassOf<UUserWidget>();
}
