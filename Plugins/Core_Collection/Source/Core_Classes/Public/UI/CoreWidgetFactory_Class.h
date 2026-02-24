// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "UI/CoreWidgetFactory.h"
#include "Templates/SubclassOf.h"
#include "UObject/SoftObjectPtr.h"

#include "CoreWidgetFactory_Class.generated.h"

class UObject;
class UUserWidget;

UCLASS()
class CORE_CLASSES_API UCoreWidgetFactory_Class : public UCoreWidgetFactory
{
	GENERATED_BODY()

public:
	UCoreWidgetFactory_Class() { }

	virtual TSubclassOf<UUserWidget> FindWidgetClassForData_Implementation(const UObject* Data) const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = ListEntries, meta = (AllowAbstract))
	TMap<TSoftClassPtr<UObject>, TSubclassOf<UUserWidget>> EntryWidgetForClass;
};
