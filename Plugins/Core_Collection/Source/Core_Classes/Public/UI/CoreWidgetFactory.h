// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "UObject/Object.h"

#include "CoreWidgetFactory.generated.h"

template <class TClass> class TSubclassOf;

class UUserWidget;
struct FFrame;

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class CORE_CLASSES_API UCoreWidgetFactory : public UObject
{
	GENERATED_BODY()

public:
	UCoreWidgetFactory() { }

	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<UUserWidget> FindWidgetClassForData(const UObject* Data) const;
};
