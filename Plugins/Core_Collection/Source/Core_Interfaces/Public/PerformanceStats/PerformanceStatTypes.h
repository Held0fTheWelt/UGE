// Copyright Epic Games, Inc. All Rights Reserved.
// Plan Paket 4: Performance-Stat-Typen in Core_Interfaces, damit Core_Classes und Core_Data nicht von Core_Performance abhängen.

#pragma once

#include "CoreMinimal.h"
#include "Misc/EnumRange.h"
#include "GameplayTagContainer.h"

#include "PerformanceStatTypes.generated.h"

UENUM(BlueprintType)
enum class ECoreStatDisplayMode : uint8
{
	Hidden,
	TextOnly,
	GraphOnly,
	TextAndGraph
};

UENUM(BlueprintType)
enum class ECoreDisplayablePerformanceStat : uint8
{
	ClientFPS,
	ServerFPS,
	IdleTime,
	FrameTime,
	FrameTime_GameThread,
	FrameTime_RenderThread,
	FrameTime_RHIThread,
	FrameTime_GPU,
	Ping,
	PacketLoss_Incoming,
	PacketLoss_Outgoing,
	PacketRate_Incoming,
	PacketRate_Outgoing,
	PacketSize_Incoming,
	PacketSize_Outgoing,
	Latency_Total,
	Latency_Game,
	Latency_Render,
	Count UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(ECoreDisplayablePerformanceStat, ECoreDisplayablePerformanceStat::Count);

USTRUCT()
struct CORE_INTERFACES_API FCorePerformanceStatGroup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Categories = "Input,Platform.Trait"))
	FGameplayTagQuery VisibilityQuery;

	UPROPERTY(EditAnywhere)
	TSet<ECoreDisplayablePerformanceStat> AllowedStats;
};
