// Copyright Epic Games, Inc. All Rights Reserved.
// Core_Collection

#include "CorePerformanceStatSubsystem.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/NetConnection.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/PerformanceStatProviderInterface.h"
#include "CorePerformanceStatTypes.h"
#include "Performance/LatencyMarkerModule.h"
#include "ProfilingDebugging/CsvProfiler.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CorePerformanceStatSubsystem)

CSV_DEFINE_CATEGORY(CorePerformance, /*bIsEnabledByDefault=*/false);

FCorePerformanceStatCache::FCorePerformanceStatCache(UCorePerformanceStatSubsystem* InSubsystem)
	: MySubsystem(InSubsystem)
{
}

void FCorePerformanceStatCache::StartCharting()
{
}

void FCorePerformanceStatCache::ProcessFrame(const FFrameData& FrameData)
{
	{
		RecordStat(ECoreDisplayablePerformanceStat::ClientFPS, (FrameData.TrueDeltaSeconds != 0.0) ? 1.0 / FrameData.TrueDeltaSeconds : 0.0);
		RecordStat(ECoreDisplayablePerformanceStat::IdleTime, FrameData.IdleSeconds);
		RecordStat(ECoreDisplayablePerformanceStat::FrameTime, FrameData.TrueDeltaSeconds);
		RecordStat(ECoreDisplayablePerformanceStat::FrameTime_GameThread, FrameData.GameThreadTimeSeconds);
		RecordStat(ECoreDisplayablePerformanceStat::FrameTime_RenderThread, FrameData.RenderThreadTimeSeconds);
		RecordStat(ECoreDisplayablePerformanceStat::FrameTime_RHIThread, FrameData.RHIThreadTimeSeconds);
		RecordStat(ECoreDisplayablePerformanceStat::FrameTime_GPU, FrameData.GPUTimeSeconds);
	}

	if (UWorld* World = MySubsystem->GetGameInstance()->GetWorld())
	{
		if (AGameStateBase* GameState = World->GetGameState<AGameStateBase>())
		{
			if (GameState->GetClass()->ImplementsInterface(UPerformanceStatProviderInterface::StaticClass()))
			{
				if (IPerformanceStatProviderInterface* PerfProvider = Cast<IPerformanceStatProviderInterface>(GameState))
				{
					RecordStat(ECoreDisplayablePerformanceStat::ServerFPS, PerfProvider->GetServerFPS());
				}
			}
		}

		if (APlayerController* LocalPC = GEngine->GetFirstLocalPlayerController(World))
		{
			if (APlayerState* PS = LocalPC->GetPlayerState<APlayerState>())
			{
				RecordStat(ECoreDisplayablePerformanceStat::Ping, PS->GetPingInMilliseconds());
			}

			if (UNetConnection* NetConnection = LocalPC->GetNetConnection())
			{
				const UNetConnection::FNetConnectionPacketLoss& InLoss = NetConnection->GetInLossPercentage();
				RecordStat(ECoreDisplayablePerformanceStat::PacketLoss_Incoming, InLoss.GetAvgLossPercentage());
				const UNetConnection::FNetConnectionPacketLoss& OutLoss = NetConnection->GetOutLossPercentage();
				RecordStat(ECoreDisplayablePerformanceStat::PacketLoss_Outgoing, OutLoss.GetAvgLossPercentage());
				RecordStat(ECoreDisplayablePerformanceStat::PacketRate_Incoming, NetConnection->InPacketsPerSecond);
				RecordStat(ECoreDisplayablePerformanceStat::PacketRate_Outgoing, NetConnection->OutPacketsPerSecond);
				RecordStat(ECoreDisplayablePerformanceStat::PacketSize_Incoming, (NetConnection->InPacketsPerSecond != 0) ? (NetConnection->InBytesPerSecond / (float)NetConnection->InPacketsPerSecond) : 0.0f);
				RecordStat(ECoreDisplayablePerformanceStat::PacketSize_Outgoing, (NetConnection->OutPacketsPerSecond != 0) ? (NetConnection->OutBytesPerSecond / (float)NetConnection->OutPacketsPerSecond) : 0.0f);
			}

			TArray<ILatencyMarkerModule*> LatencyMarkerModules = IModularFeatures::Get().GetModularFeatureImplementations<ILatencyMarkerModule>(ILatencyMarkerModule::GetModularFeatureName());
			for (ILatencyMarkerModule* LatencyMarkerModule : LatencyMarkerModules)
			{
				if (LatencyMarkerModule->GetEnabled())
				{
					const float TotalLatencyMs = LatencyMarkerModule->GetTotalLatencyInMs();
					if (TotalLatencyMs > 0.0f)
					{
						RecordStat(ECoreDisplayablePerformanceStat::Latency_Total, TotalLatencyMs);
						RecordStat(ECoreDisplayablePerformanceStat::Latency_Game, LatencyMarkerModule->GetGameLatencyInMs());
						RecordStat(ECoreDisplayablePerformanceStat::Latency_Render, LatencyMarkerModule->GetRenderLatencyInMs());
#if CSV_PROFILER
						if (FCsvProfiler* Profiler = FCsvProfiler::Get())
						{
							static const FName TotalLatencyStatName = TEXT("Core_Latency_Total");
							Profiler->RecordCustomStat(TotalLatencyStatName, CSV_CATEGORY_INDEX(CorePerformance), TotalLatencyMs, ECsvCustomStatOp::Set);
							static const FName GameLatencyStatName = TEXT("Core_Latency_Game");
							Profiler->RecordCustomStat(GameLatencyStatName, CSV_CATEGORY_INDEX(CorePerformance), LatencyMarkerModule->GetGameLatencyInMs(), ECsvCustomStatOp::Set);
							static const FName RenderLatencyStatName = TEXT("Core_Latency_Render");
							Profiler->RecordCustomStat(RenderLatencyStatName, CSV_CATEGORY_INDEX(CorePerformance), LatencyMarkerModule->GetRenderLatencyInMs(), ECsvCustomStatOp::Set);
						}
#endif
						break;
					}
				}
			}
		}
	}
}

void FCorePerformanceStatCache::StopCharting()
{
}

void FCorePerformanceStatCache::RecordStat(const ECoreDisplayablePerformanceStat Stat, const double Value)
{
	PerfStateCache.FindOrAdd(Stat).RecordSample(Value);
}

double FCorePerformanceStatCache::GetCachedStat(ECoreDisplayablePerformanceStat Stat) const
{
	static_assert((int32)ECoreDisplayablePerformanceStat::Count == 18, "Need to update this function to deal with new performance stats");
	if (const FSampledStatCache* Cache = GetCachedStatData(Stat))
		return Cache->GetLastCachedStat();
	return 0.0;
}

const FSampledStatCache* FCorePerformanceStatCache::GetCachedStatData(const ECoreDisplayablePerformanceStat Stat) const
{
	static_assert((int32)ECoreDisplayablePerformanceStat::Count == 18, "Need to update this function to deal with new performance stats");
	return PerfStateCache.Find(Stat);
}

void UCorePerformanceStatSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Tracker = MakeShared<FCorePerformanceStatCache>(this);
	GEngine->AddPerformanceDataConsumer(Tracker);
}

void UCorePerformanceStatSubsystem::Deinitialize()
{
	GEngine->RemovePerformanceDataConsumer(Tracker);
	Tracker.Reset();
}

double UCorePerformanceStatSubsystem::GetCachedStat(ECoreDisplayablePerformanceStat Stat) const
{
	return Tracker->GetCachedStat(Stat);
}

const FSampledStatCache* UCorePerformanceStatSubsystem::GetCachedStatData(const ECoreDisplayablePerformanceStat Stat) const
{
	return Tracker->GetCachedStatData(Stat);
}

void UCorePerformanceStatSubsystem::GetCachedStatSamples(ECoreDisplayablePerformanceStat Stat, TArray<double>& OutSamples) const
{
	OutSamples.Reset();
	if (const FSampledStatCache* Cache = Tracker->GetCachedStatData(Stat))
	{
		OutSamples.Reserve(Cache->GetSampleSize());
		Cache->ForEachCurrentSample([&OutSamples](const double Value) { OutSamples.Add(Value); });
	}
}
