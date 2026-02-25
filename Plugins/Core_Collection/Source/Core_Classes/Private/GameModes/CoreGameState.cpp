// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#include "GameModes/CoreGameState.h"

#include "Characters/CoreCharacter.h"
#include "AbilitySystemComponent.h"
#include "Interfaces/AbilitySystemComponentClassRegistry.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Structs/VerbMessageTypes.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CoreGameState)

class FLifetimeProperty;

extern ENGINE_API float GAverageFPS;

ACoreGameState::ACoreGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	UClass* ASCClass = CoreAbilitySystemComponentClassRegistry::GetAbilitySystemComponentClass();
	if (ASCClass == nullptr)
	{
		ASCClass = UAbilitySystemComponent::StaticClass();
	}
	AbilitySystemComponent = Cast<UAbilitySystemComponent>(ObjectInitializer.CreateDefaultSubobject(this, TEXT("AbilitySystemComponent"), UAbilitySystemComponent::StaticClass(), ASCClass, /*bIsRequired=*/ true, /*bIsTransient=*/ false));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ServerFPS = 0.0f;
}

void ACoreGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ACoreGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(/*Owner=*/ this, /*Avatar=*/ this);
}

UAbilitySystemComponent* ACoreGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACoreGameState::InitAbilityActorInfo(UObject* InOwnerActor, UObject* InAvatarActor)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(Cast<AActor>(InOwnerActor), Cast<AActor>(InAvatarActor));
	}
}

void ACoreGameState::ClearActorInfo()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->ClearActorInfo();
	}
}

void ACoreGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACoreGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
}

void ACoreGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
}

void ACoreGameState::SeamlessTravelTransitionCheckpoint(bool bToTransitionMap)
{
	for (int32 i = PlayerArray.Num() - 1; i >= 0; i--)
	{
		APlayerState* PlayerState = PlayerArray[i];
		if (PlayerState && (PlayerState->IsABot() || PlayerState->IsInactive()))
		{
			RemovePlayerState(PlayerState);
		}
	}
}

void ACoreGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ServerFPS);
	DOREPLIFETIME_CONDITION(ThisClass, RecorderPlayerState, COND_ReplayOnly);
}

void ACoreGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetLocalRole() == ROLE_Authority)
	{
		ServerFPS = GAverageFPS;
	}
}

void ACoreGameState::MulticastMessageToClients_Implementation(const FVerbMessage Message)
{
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

void ACoreGameState::MulticastReliableMessageToClients_Implementation(const FVerbMessage Message)
{
	MulticastMessageToClients_Implementation(Message);
}

float ACoreGameState::GetServerFPS() const
{
	return ServerFPS;
}

UClass* ACoreGameState::GetReplicationGraphCharacterClass() const
{
	return ACoreCharacter::StaticClass();
}

void ACoreGameState::SetRecorderPlayerState(APlayerState* NewPlayerState)
{
	if (RecorderPlayerState == nullptr)
	{
		RecorderPlayerState = NewPlayerState;
		OnRep_RecorderPlayerState();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetRecorderPlayerState was called on %s but should only be called once per game on the primary user"), *GetName());
	}
}

APlayerState* ACoreGameState::GetRecorderPlayerState() const
{
	return RecorderPlayerState;
}

void ACoreGameState::OnRep_RecorderPlayerState()
{
	OnRecorderPlayerStateChangedEvent.Broadcast(RecorderPlayerState);
}
