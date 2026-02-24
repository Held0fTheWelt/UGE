// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "UI/CorePerfStatWidgetBase.h"

#include "Engine/GameInstance.h"
#include "Styling/CoreStyle.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CorePerfStatWidgetBase)

void SLatencyGraph::Construct(const FArguments& InArgs)
{
	DesiredSize = InArgs._DesiredSize;
	MaxYAxisOfGraph = InArgs._MaxLatencyToGraph;
	LineColor = InArgs._LineColor;
	BackgroundColor = InArgs._BackgroundColor;
}

int32 SLatencyGraph::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 MaxLayerId = LayerId;

	// Draw the background
	FSlateDrawElement::MakeRotatedBox(
		OutDrawElements,
		MaxLayerId,
		AllottedGeometry.ToPaintGeometry(),
		FCoreStyle::Get().GetBrush("BlackBrush"),
		ESlateDrawEffect::NoPixelSnapping,
		0,
		TOptional<FVector2D>(),
		FSlateDrawElement::RelativeToElement,
		BackgroundColor);

	// We need to actually draw the graph plot on top of the background
	// so increment the layer
	MaxLayerId++;

	// Actually draw the graph plot
	DrawTotalLatency(AllottedGeometry, OutDrawElements, MaxLayerId);
	
	MaxLayerId++;

	return MaxLayerId;
}

FVector2D SLatencyGraph::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return DesiredSize;
}

void SLatencyGraph::DrawTotalLatency(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
	if (GraphSamples.Num() == 0)
	{
		return;
	}
	
	static TArray<FVector2D> Points;
	Points.Reset(GraphSamples.Num() + 1);
	
	const FVector2D WidgetSize = AllottedGeometry.GetLocalSize();
	const float LineThickness = 1.0f;
	const double XSlice = WidgetSize.X / static_cast<double>(GraphSamples.Num());
	const double Border = 1.0;

	for (int32 i = 0; i < GraphSamples.Num(); ++i)
	{
		const double Stat = GraphSamples[i];
		double Y = WidgetSize.Y - FMath::Clamp((Stat * ScaleFactor), 0.0, MaxYAxisOfGraph) / MaxYAxisOfGraph * WidgetSize.Y;
		Y = FMath::Clamp(Y, Border, WidgetSize.Y - Border);
		Points.Emplace(XSlice * double(i + 1), Y);
	}

	// Why does this not just draw a straight line?? 
	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		Points,
		ESlateDrawEffect::NoPixelSnapping,
		LineColor,
		false,
		LineThickness);
}

//////////////////////////////////////////////////////////////////////
// UCorePerfStatGraph

UCorePerfStatGraph::UCorePerfStatGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetClipping(EWidgetClipping::ClipToBounds);
}

TSharedRef<SWidget> UCorePerfStatGraph::RebuildWidget()
{
	return SAssignNew(SlateLatencyGraph, SLatencyGraph);
}

void UCorePerfStatGraph::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	SlateLatencyGraph.Reset();
}

void UCorePerfStatGraph::SetLineColor(const FColor& InColor)
{
	if (SlateLatencyGraph.IsValid())
	{
		SlateLatencyGraph->SetLineColor(InColor);
	}
}

void UCorePerfStatGraph::SetMaxYValue(const float InValue)
{
	if (SlateLatencyGraph.IsValid())
	{
		SlateLatencyGraph->SetMaxYValue(InValue);
	}
}

void UCorePerfStatGraph::SetBackgroundColor(const FColor& InValue)
{
	if (SlateLatencyGraph.IsValid())
	{
		SlateLatencyGraph->SetBackgroundColor(InValue);
	}
}

void UCorePerfStatGraph::UpdateGraphData(const TArray<double>& Samples, const float ScaleFactor)
{
	if (SlateLatencyGraph.IsValid())
	{
		SlateLatencyGraph->UpdateGraphData(Samples, ScaleFactor);
	}
}

//////////////////////////////////////////////////////////////////////
// UCorePerfStatWidgetBase

double UCorePerfStatWidgetBase::FetchStatValue()
{
	if (UPerformanceStatSubsystemBase* Subsystem = GetStatSubsystem())
	{
		return CachedStatSubsystem->GetCachedStat(StatToDisplay);
	}
	else
	{
		return 0.0;
	}
}

void UCorePerfStatWidgetBase::UpdateGraphData(const float ScaleFactor)
{
	if (PerfStatGraph && CachedStatSubsystem)
	{
		TArray<double> Samples;
		CachedStatSubsystem->GetCachedStatSamples(StatToDisplay, Samples);
		if (Samples.Num() > 0)
		{
			PerfStatGraph->UpdateGraphData(Samples, ScaleFactor);
		}
	}
}

void UCorePerfStatWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	// Cache the subsystem on construct, which will also make sure the graph is up to date
	GetStatSubsystem();
	
	if (PerfStatGraph)
	{
		PerfStatGraph->SetLineColor(GraphLineColor);
		PerfStatGraph->SetMaxYValue(GraphMaxYValue);
		PerfStatGraph->SetBackgroundColor(GraphBackgroundColor);
	}
}

UPerformanceStatSubsystemBase* UCorePerfStatWidgetBase::GetStatSubsystem()
{
	if (CachedStatSubsystem == nullptr)
	{
		if (UWorld* World = GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance())
			{
				CachedStatSubsystem = GameInstance->GetSubsystem<UPerformanceStatSubsystemBase>();
			}
		}
	}

	return CachedStatSubsystem;
}
