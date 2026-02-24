// Copyright Epic Games, Inc. All Rights Reserved.
// Core_Collection

#pragma once

#include "ChartCreation.h"
#include "CorePerformanceStatTypes.h"
#include "Subsystems/PerformanceStatSubsystemBase.h"
#include "Algo/MaxElement.h"
#include "Algo/MinElement.h"
#include "Stats/StatsData.h"

#include "CorePerformanceStatSubsystem.generated.h"

class FSubsystemCollectionBase;
class UCorePerformanceStatSubsystem;
class UObject;
struct FFrame;

class CORE_PERFORMANCE_API FSampledStatCache
{
public:
	FSampledStatCache(const int32 InSampleSize = 125)
		: SampleSize(InSampleSize)
	{
		check(InSampleSize > 0);
		Samples.Empty();
		Samples.AddZeroed(SampleSize);
	}
	void RecordSample(const double Sample)
	{
		Samples[CurrentSampleIndex] = Sample;
		CurrentSampleIndex++;
		if (CurrentSampleIndex >= Samples.Num()) { CurrentSampleIndex = 0u; }
	}
	double GetCurrentCachedStat() const { return Samples[CurrentSampleIndex]; }
	double GetLastCachedStat() const
	{
		int32 LastIndex = CurrentSampleIndex - 1;
		if (LastIndex < 0) { LastIndex = Samples.Num() - 1; }
		return Samples[LastIndex];
	}
	void ForEachCurrentSample(const TFunctionRef<void(const double Stat)> Func) const
	{
		int32 Index = CurrentSampleIndex;
		for (int32 i = 0; i < SampleSize; i++) { Func(Samples[Index]); Index++; if (Index == SampleSize) Index = 0; }
	}
	inline int32 GetSampleSize() const { return SampleSize; }
	inline double GetAverage() const { double Sum = 0.0; ForEachCurrentSample([&Sum](const double Stat) { Sum += Stat; }); return Sum / static_cast<double>(SampleSize); }
	inline double GetMin() const { return *Algo::MinElement(Samples); }
	inline double GetMax() const { return *Algo::MaxElement(Samples); }
private:
	const int32 SampleSize = 125;
	int32 CurrentSampleIndex = 0;
	TArray<double> Samples;
};

struct CORE_PERFORMANCE_API FCorePerformanceStatCache : public IPerformanceDataConsumer
{
public:
	FCorePerformanceStatCache(UCorePerformanceStatSubsystem* InSubsystem);

	virtual void StartCharting() override;
	virtual void ProcessFrame(const FFrameData& FrameData) override;
	virtual void StopCharting() override;

	double GetCachedStat(ECoreDisplayablePerformanceStat Stat) const;
	const FSampledStatCache* GetCachedStatData(const ECoreDisplayablePerformanceStat Stat) const;

protected:
	void RecordStat(const ECoreDisplayablePerformanceStat Stat, const double Value);
	UCorePerformanceStatSubsystem* MySubsystem;
	TMap<ECoreDisplayablePerformanceStat, FSampledStatCache> PerfStateCache;
};

UCLASS(BlueprintType)
class CORE_PERFORMANCE_API UCorePerformanceStatSubsystem : public UPerformanceStatSubsystemBase
{
	GENERATED_BODY()

public:
	virtual double GetCachedStat(ECoreDisplayablePerformanceStat Stat) const override;

	virtual void GetCachedStatSamples(ECoreDisplayablePerformanceStat Stat, TArray<double>& OutSamples) const override;

	const FSampledStatCache* GetCachedStatData(const ECoreDisplayablePerformanceStat Stat) const;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:
	TSharedPtr<FCorePerformanceStatCache> Tracker;
};
