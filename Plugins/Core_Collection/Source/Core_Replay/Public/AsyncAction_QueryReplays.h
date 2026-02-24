// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"

#include "UObject/ObjectPtr.h"
#include "AsyncAction_QueryReplays.generated.h"

class APlayerController;
class INetworkReplayStreamer;
class UWarCollectionReplayList;
class UObject;
struct FEnumerateStreamsResult;
struct FFrame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQueryReplayAsyncDelegate, UWarCollectionReplayList*, Results);

/**
 * Async action to query for available replays
 */
UCLASS()
class CORE_REPLAY_API UAsyncAction_QueryReplays : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UAsyncAction_QueryReplays(const FObjectInitializer& ObjectInitializer);

	// Queries for available replays
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static CORE_REPLAY_API UAsyncAction_QueryReplays* QueryReplays(APlayerController* PlayerController);

	virtual void Activate() override;

public:
	// Called when the replay query completes
	UPROPERTY(BlueprintAssignable)
	FQueryReplayAsyncDelegate QueryComplete;

private:
	void OnEnumerateStreamsComplete(const FEnumerateStreamsResult& Result);

private:
	UPROPERTY()
	TObjectPtr<UWarCollectionReplayList> ResultList;

	TWeakObjectPtr<APlayerController> PlayerController;

	TSharedPtr<INetworkReplayStreamer> ReplayStreamer;
};
