// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CommonActivatableWidget.h"

#include "CoreActivatableWidget.generated.h"

struct FUIInputConfig;

/**
 * Input mode for widgets
 */
UENUM(BlueprintType)
enum class ECoreWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

/**
 * UCoreActivatableWidget
 *
 * An activatable widget that automatically drives the desired input config when activated.
 * Adapted from ULyraActivatableWidget.
 */
UCLASS(Abstract, Blueprintable)
class CORE_CLASSES_API UCoreActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UCoreActivatableWidget(const FObjectInitializer& ObjectInitializer);
	
public:
	
	//~UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface

#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif
	
protected:
	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	ECoreWidgetInputMode InputConfig = ECoreWidgetInputMode::Default;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
