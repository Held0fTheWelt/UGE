// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "NetworkReplayStreaming.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"

#include "WarCollectionReplaySubsystem.generated.h"

class UDemoNetDriver;
class APlayerController;
class ULocalPlayer;
struct FFrame;

/** An available replay for display in the UI */
UCLASS(MinimalAPI, BlueprintType)
class CORE_REPLAY_API UWarCollectionReplayListEntry : public UObject
{
	GENERATED_BODY()

public:
	FNetworkReplayStreamInfo StreamInfo;

	/** The UI friendly name of the stream */
	UFUNCTION(BlueprintPure, Category=Replays)
	FString GetFriendlyName() const { return StreamInfo.FriendlyName; }

	/** The date and time the stream was recorded */
	UFUNCTION(BlueprintPure, Category=Replays)
	FDateTime GetTimestamp() const { return StreamInfo.Timestamp; }

	/** The duration of the stream in MS */
	UFUNCTION(BlueprintPure, Category=Replays)
	FTimespan GetDuration() const { return FTimespan::FromMilliseconds(StreamInfo.LengthInMS); }

	/** Number of viewers viewing this stream */
	UFUNCTION(BlueprintPure, Category=Replays)
	int32 GetNumViewers() const { return StreamInfo.NumViewers; }

	/** True if the stream is live and the game hasn't completed yet */
	UFUNCTION(BlueprintPure, Category=Replays)
	bool GetIsLive() const { return StreamInfo.bIsLive; }
};

/** Results of querying for replays list of results for the UI */
UCLASS(MinimalAPI, BlueprintType)
class CORE_REPLAY_API UWarCollectionReplayList : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category=Replays)
	TArray<TObjectPtr<UWarCollectionReplayListEntry>> Results;
};

/** Subsystem to handle recording/loading replays */
UCLASS(MinimalAPI)
class CORE_REPLAY_API UWarCollectionReplaySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	CORE_REPLAY_API UWarCollectionReplaySubsystem();

	/** Returns true if this platform supports replays at all */
	UFUNCTION(BlueprintCallable, Category = Replays, BlueprintPure = false)
	static CORE_REPLAY_API bool DoesPlatformSupportReplays();

	/** Returns the trait tag for platform support, used in options */
	static CORE_REPLAY_API FGameplayTag GetPlatformSupportTraitTag();

	/** Loads the appropriate map and plays a replay */
	UFUNCTION(BlueprintCallable, Category=Replays)
	CORE_REPLAY_API void PlayReplay(UWarCollectionReplayListEntry* Replay);

	/** Starts recording a client replay, and handles any file cleanup needed */
	UFUNCTION(BlueprintCallable, Category = Replays)
	CORE_REPLAY_API void RecordClientReplay(APlayerController* PlayerController);

	/** Starts deleting local replays starting with the oldest until there are NumReplaysToKeep or fewer */
	UFUNCTION(BlueprintCallable, Category = Replays)
	CORE_REPLAY_API void CleanupLocalReplays(ULocalPlayer* LocalPlayer, int32 NumReplaysToKeep);

	/** Move forward or back in currently playing replay */
	UFUNCTION(BlueprintCallable, Category=Replays)
	CORE_REPLAY_API void SeekInActiveReplay(float TimeInSeconds);

	/** Gets length of current replay */
	UFUNCTION(BlueprintCallable, Category = Replays, BlueprintPure = false)
	CORE_REPLAY_API float GetReplayLengthInSeconds() const;

	/** Gets current playback time */
	UFUNCTION(BlueprintCallable, Category=Replays, BlueprintPure=false)
	CORE_REPLAY_API float GetReplayCurrentTime() const;

private:
	TSharedPtr<INetworkReplayStreamer> CurrentReplayStreamer;

	UPROPERTY()
	TObjectPtr<ULocalPlayer> LocalPlayerDeletingReplays;

	int32 DeletingReplaysNumberToKeep;

	UDemoNetDriver* GetDemoDriver() const;

	void OnEnumerateStreamsCompleteForDelete(const FEnumerateStreamsResult& Result);
	void OnDeleteReplay(const FDeleteFinishedStreamResult& DeleteResult);
};
