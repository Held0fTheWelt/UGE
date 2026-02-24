// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "Components/GameplayMessageProcessor.h"

#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/GameStateBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayMessageProcessor)

void UGameplayMessageProcessor::BeginPlay()
{
	Super::BeginPlay();
	StartListening();
}

void UGameplayMessageProcessor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopListening();
	Super::EndPlay(EndPlayReason);
}

void UGameplayMessageProcessor::StartListening()
{
	// Override in derived classes to add listeners
}

void UGameplayMessageProcessor::StopListening()
{
	for (FGameplayMessageListenerHandle& Handle : ListenerHandles)
	{
		Handle.Unregister();
	}
	ListenerHandles.Empty();
}

void UGameplayMessageProcessor::AddListenerHandle(FGameplayMessageListenerHandle&& Handle)
{
	ListenerHandles.Add(MoveTemp(Handle));
}

double UGameplayMessageProcessor::GetServerTime() const
{
	if (AGameStateBase* GameState = GetWorld()->GetGameState())
	{
		return GameState->GetServerWorldTimeSeconds();
	}
	return 0.0;
}
