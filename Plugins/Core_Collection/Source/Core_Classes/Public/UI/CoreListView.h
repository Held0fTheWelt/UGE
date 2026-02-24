// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CommonListView.h"

#include "CoreListView.generated.h"

class UUserWidget;
class ULocalPlayer;
class UCoreWidgetFactory;

/**
 * UCoreListView
 *
 * List view with widget factory integration.
 * Adapted from ULyraListView.
 */
UCLASS(meta = (DisableNativeTick))
class CORE_CLASSES_API UCoreListView : public UCommonListView
{
	GENERATED_BODY()

public:
	UCoreListView(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& InCompileLog) const override;
#endif

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;

protected:
	UPROPERTY(EditAnywhere, Instanced, Category="Entry Creation")
	TArray<TObjectPtr<UCoreWidgetFactory>> FactoryRules;
};
