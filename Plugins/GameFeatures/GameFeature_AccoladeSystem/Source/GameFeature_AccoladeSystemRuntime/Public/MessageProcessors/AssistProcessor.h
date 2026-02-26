#pragma once

#include "Components/GameplayMessageProcessor.h"

#include "AssistProcessor.generated.h"

class APlayerState;
struct FGameplayTag;
struct FVerbMessage;

/**
 * FPlayerAssistDamageTracking
 *
 * Accumulates damage dealt to a single player, keyed by attacker.
 */
USTRUCT()
struct FPlayerAssistDamageTracking
{
	GENERATED_BODY()

	/** Damage accumulated per attacker (PlayerState → total damage). */
	UPROPERTY(Transient)
	TMap<TObjectPtr<APlayerState>, float> AccumulatedDamageByPlayer;
};

/**
 * UAssistProcessor
 *
 * Tracks which players dealt damage to each target.
 * When a target is eliminated, grants an assist message to every attacker
 * who was NOT the final eliminator.
 *
 * Listen channels (via GameplayMessageSubsystem):
 *  - "Elimination.Message"  (FVerbMessage)
 *  - "Damage.Message"       (FVerbMessage)
 *
 * Emits:
 *  - "Assist.Message"       (FVerbMessage, Instigator = assisting player, Target = eliminated player)
 *
 * Migrated from UAssistProcessor (ShooterCore).
 * FLyraVerbMessage → FVerbMessage (Elemental_Structure).
 */
UCLASS()
class UAssistProcessor : public UGameplayMessageProcessor
{
	GENERATED_BODY()

public:

	virtual void StartListening() override;

private:

	void OnDamageMessage(FGameplayTag Channel, const FVerbMessage& Payload);
	void OnEliminationMessage(FGameplayTag Channel, const FVerbMessage& Payload);

	/** Maps eliminated player → per-attacker damage totals. */
	UPROPERTY(Transient)
	TMap<TObjectPtr<APlayerState>, FPlayerAssistDamageTracking> DamageHistory;
};
