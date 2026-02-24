// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "CoreBotCheats.h"
#include "Engine/World.h"
#include "GameFramework/CheatManagerDefines.h"
#include "GameFramework/GameStateBase.h"
#include "Interfaces/BotSpawnProviderInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreBotCheats)

//////////////////////////////////////////////////////////////////////
// UCoreBotCheats

UCoreBotCheats::UCoreBotCheats()
{
#if WITH_SERVER_CODE && UE_WITH_CHEAT_MANAGER
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		UCheatManager::RegisterForOnCheatManagerCreated(FOnCheatManagerCreated::FDelegate::CreateLambda(
			[](UCheatManager* CheatManager)
			{
				CheatManager->AddCheatManagerExtension(NewObject<ThisClass>(CheatManager));
			}));
	}
#endif
}

void UCoreBotCheats::AddPlayerBot()
{
#if WITH_SERVER_CODE && UE_WITH_CHEAT_MANAGER
	if (IBotSpawnProviderInterface* Provider = GetBotSpawnProvider())
	{
		Provider->SpawnOneBot();
	}
#endif	
}

void UCoreBotCheats::RemovePlayerBot()
{
#if WITH_SERVER_CODE && UE_WITH_CHEAT_MANAGER
	if (IBotSpawnProviderInterface* Provider = GetBotSpawnProvider())
	{
		Provider->RemoveOneBot();
	}
#endif	
}

IBotSpawnProviderInterface* UCoreBotCheats::GetBotSpawnProvider() const
{
	if (UWorld* World = GetWorld())
	{
		if (AGameStateBase* GameState = World->GetGameState())
		{
			if (UActorComponent* Comp = GameState->FindComponentByInterface(UBotSpawnProviderInterface::StaticClass()))
			{
				return Cast<IBotSpawnProviderInterface>(Comp);
			}
		}
	}
	return nullptr;
}
