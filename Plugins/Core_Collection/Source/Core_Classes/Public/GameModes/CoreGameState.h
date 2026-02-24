// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

#pragma once

#include "AbilitySystemInterface.h"
#include "Elemental_Classes/Public/States/ModularGameState.h"
#include "Structs/VerbMessageTypes.h"
#include "Interfaces/PerformanceStatProviderInterface.h"
#include "Interfaces/ReplicationGraphCharacterProviderInterface.h"

#include "CoreGameState.generated.h"

class APlayerState;
class UAbilitySystemComponent;
class UObject;
struct FFrame;

/**
 * ACoreGameState
 *
 *	The base game state class used by this project.
 *	Adapted from ALyraGameState.
 */
UCLASS(Config = Game)
class CORE_CLASSES_API ACoreGameState : public AModularGameStateBase, public IAbilitySystemInterface, public IPerformanceStatProviderInterface, public IReplicationGraphCharacterProviderInterface
{
	GENERATED_BODY()

public:

	ACoreGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
	//~End of AActor interface

	//~AGameStateBase interface
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	virtual void SeamlessTravelTransitionCheckpoint(bool bToTransitionMap) override;
	//~End of AGameStateBase interface

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface

	void InitAbilityActorInfo(UObject* InOwnerActor, UObject* InAvatarActor);
	void ClearActorInfo();

	// Gets the ability system component used for game wide things
	UFUNCTION(BlueprintCallable, Category = "Core|GameState")
	UAbilitySystemComponent* GetCoreAbilitySystemComponent() const { return AbilitySystemComponent; }

	// Send a message that all clients will (probably) get
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "Core|GameState")
	void MulticastMessageToClients(const FVerbMessage Message);

	// Send a message that all clients will be guaranteed to get
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Core|GameState")
	void MulticastReliableMessageToClients(const FVerbMessage Message);

	// Gets the server's FPS, replicated to clients (IPerformanceStatProviderInterface)
	UFUNCTION(BlueprintCallable, Category = "Performance")
	virtual float GetServerFPS() const override;

	//~IReplicationGraphCharacterProviderInterface
	virtual UClass* GetReplicationGraphCharacterClass() const override;
	//~End of IReplicationGraphCharacterProviderInterface

	// Indicate the local player state is recording a replay
	void SetRecorderPlayerState(APlayerState* NewPlayerState);

	// Gets the player state that recorded the replay, if valid
	APlayerState* GetRecorderPlayerState() const;

	// Delegate called when the replay player state changes
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRecorderPlayerStateChanged, APlayerState*);
	FOnRecorderPlayerStateChanged OnRecorderPlayerStateChangedEvent;

private:
	UPROPERTY(VisibleAnywhere, Category = "Core|GameState")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

protected:
	UPROPERTY(Replicated)
	float ServerFPS;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_RecorderPlayerState)
	TObjectPtr<APlayerState> RecorderPlayerState;

	UFUNCTION()
	void OnRep_RecorderPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
