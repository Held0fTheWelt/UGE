// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "CommonUserWidget.h"
#include "Widgets/SLeafWidget.h"
#include "PerformanceStats/PerformanceStatTypes.h"
#include "Subsystems/PerformanceStatSubsystemBase.h"

#include "CorePerfStatWidgetBase.generated.h"

class UObject;
struct FFrame;

/**
 * Slate widget for drawing latency graphs
 */
class SLatencyGraph : public SLeafWidget
{
public:
	/** Begin the arguments for this slate widget */
	SLATE_BEGIN_ARGS(SLatencyGraph)
		: _DesiredSize(150, 50),
		_MaxLatencyToGraph(33.0),
		_LineColor(255, 255, 255, 255),
		_BackgroundColor(0, 0, 0, 128)
	{
		_Clipping = EWidgetClipping::ClipToBounds;
	}

	SLATE_ARGUMENT(FVector2D, DesiredSize)
	SLATE_ARGUMENT(double, MaxLatencyToGraph)
	SLATE_ARGUMENT(FColor, LineColor)
	SLATE_ARGUMENT(FColor, BackgroundColor)
	SLATE_END_ARGS()

	/** Contruct function needed for every Widget */
	void Construct(const FArguments& InArgs);

	/** Called with the elements to be drawn */
	virtual int32 OnPaint(const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyClippingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;

	virtual bool ComputeVolatility() const override { return true; }
	
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

	inline void SetLineColor(const FColor& InColor)
	{
		LineColor = InColor;	
	}

	inline void SetMaxYValue(const double InValue)
	{
		MaxYAxisOfGraph = InValue;
	}

	inline void SetBackgroundColor(const FColor& InColor)
	{
		BackgroundColor = InColor;
	}

	inline void UpdateGraphData(const TArray<double>& InSamples, const float InScaleFactor)
	{
		GraphSamples = InSamples;
		ScaleFactor = InScaleFactor;
	}
	
private:
	
	void DrawTotalLatency(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;

	/**
	 * The size of the graph to draw
	 */
	FVector2D DesiredSize = { 150.0, 50.0 };

	/**
	 * Max Y value of the graph. The values drawn will be clamped to this
	 */
	double MaxYAxisOfGraph = 33.0;

	float ScaleFactor = 1.0f;

	/**
	 * Color of the line to draw on the graph
	 */
	FColor LineColor = FColor(255, 255, 255, 255);

	/**
	 * The background color to draw when drawing the graph
	 */
	FColor BackgroundColor = FColor(0, 0, 0, 128);

	/**
	 * Sample data for the graph (copy of current samples from performance stat subsystem).
	 */
	TArray<double> GraphSamples;
};

/**
 * UCorePerfStatGraph
 *
 * Base class for a widget that displays the graph of a stat over time.
 */
UCLASS(meta = (DisableNativeTick))
class CORE_CLASSES_API UCorePerfStatGraph : public UUserWidget
{
	GENERATED_BODY()

public:
	UCorePerfStatGraph(const FObjectInitializer& ObjectInitializer);
	
	void SetLineColor(const FColor& InColor);
	
	void SetMaxYValue(const float InValue);
	
	void SetBackgroundColor(const FColor& InValue);

	void UpdateGraphData(const TArray<double>& Samples, const float ScaleFactor);
	
protected:
	// Begin UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	// End UWidget interface

	// The actual slate widget which will draw the graph. Created in RebuildWidget and
	// destroyed in ReleaseSlateResources.
	TSharedPtr<SLatencyGraph> SlateLatencyGraph;
};

/**
 * UCorePerfStatWidgetBase
 *
 * Base class for a widget that displays a single stat, e.g., FPS, ping, etc...
 */
UCLASS(Abstract)
class CORE_CLASSES_API UCorePerfStatWidgetBase : public UCommonUserWidget
{
public:
	GENERATED_BODY()

public:
	// Returns the stat this widget is supposed to display
	UFUNCTION(BlueprintPure)
	ECoreDisplayablePerformanceStat GetStatToDisplay() const
	{
		return StatToDisplay;
	}

	// Polls for the value of this stat (unscaled)
	UFUNCTION(BlueprintPure)
	double FetchStatValue();

	UFUNCTION(BlueprintCallable)
	void UpdateGraphData(const float ScaleFactor = 1.0f);

protected:

 	virtual void NativeConstruct() override;

 	UPerformanceStatSubsystemBase* GetStatSubsystem();

	/**
	 * An optional stat graph widget to display this stat's value over time.
	 */
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, OptionalWidget=true))
 	TObjectPtr<UCorePerfStatGraph> PerfStatGraph;
 	
	// Cached subsystem pointer (from Core_Interfaces base; implemented by Core_Performance)
	UPROPERTY(Transient)
	TObjectPtr<UPerformanceStatSubsystemBase> CachedStatSubsystem;

 	UPROPERTY(EditAnywhere, Category = Display)
 	FColor GraphLineColor = FColor(255, 255, 255, 255);
 	
 	UPROPERTY(EditAnywhere, Category = Display)
 	FColor GraphBackgroundColor = FColor(0, 0, 0, 128);

 	/**
	  * The max value of the Y axis to clamp the graph to. 
	  */
 	UPROPERTY(EditAnywhere, Category = Display)
 	double GraphMaxYValue = 33.0;

	// The stat to display
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Display)
	ECoreDisplayablePerformanceStat StatToDisplay;
};
