#pragma once

#include "Components/GameplayMessageProcessor.h"

#include "ElimChainProcessor.generated.h"

class APlayerState;
struct FGameplayTag;
struct FVerbMessage;

/**
 * FPlayerElimChainInfo
 *
 * Persists chain state between elimination events for a single player.
 */
USTRUCT()
struct FPlayerElimChainInfo
{
	GENERATED_BODY()

	/** World time of the last elimination (used to detect chain expiry). */
	double LastEliminationTime = 0.0;

	/** How many eliminations in the current chain. */
	int32 ChainCounter = 1;
};

/**
 * UElimChainProcessor
 *
 * Abstract base. Tracks "quick successive kill" chains.
 * A chain resets if more than ChainTimeLimit seconds pass between eliminations.
 * When a chain reaches a count listed in ElimChainTags, the matching tag is broadcast.
 *
 * Listen channel: "Elimination.Message" (FVerbMessage)
 * Emits:          tag from ElimChainTags[N] (FVerbMessage) when chain length N is reached
 *
 * Migrated from UElimChainProcessor (ShooterCore).
 * Subclass in Blueprint and fill ElimChainTags to customise thresholds.
 */
UCLASS(Abstract)
class UElimChainProcessor : public UGameplayMessageProcessor
{
	GENERATED_BODY()

public:

	virtual void StartListening() override;

protected:

	/** Maximum seconds allowed between eliminations to continue the chain. */
	UPROPERTY(EditDefaultsOnly, Category = "Accolade|Chain")
	float ChainTimeLimit = 4.5f;

	/**
	 * Map of chain length → broadcast tag.
	 * Example: { 2 → "Accolade.DoubleElim", 3 → "Accolade.TripleElim" }
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Accolade|Chain")
	TMap<int32, FGameplayTag> ElimChainTags;

private:

	void OnEliminationMessage(FGameplayTag Channel, const FVerbMessage& Payload);

	UPROPERTY(Transient)
	TMap<TObjectPtr<APlayerState>, FPlayerElimChainInfo> PlayerChainHistory;
};
