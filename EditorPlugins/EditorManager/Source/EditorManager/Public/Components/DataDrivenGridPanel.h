// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GridPanel.h"
#include "DataDrivenGridPanel.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Data Driven Grid Panel"), BlueprintType, Blueprintable)
class EDITORMANAGER_API UDataDrivenGridPanel : public UGridPanel
{
	GENERATED_BODY()

public:
    UDataDrivenGridPanel(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
    // Steuert, in welcher Kategorie im UMG-Palette-Panel das Widget erscheint
    virtual const FText GetPaletteCategory() override
    {
        return NSLOCTEXT("UMG", "MyCustomWidgets", "My Widgets");
    }
#endif
};
