#pragma once

#include "Components/GameplayMessageProcessor.h"

#include "ElimStreakProcessor.generated.h"

class APlayerState;
struct FGameplayTag;
struct FVerbMessage;

/**
 * UElimStreakProcessor
 *
 * Abstract base. Tracks "kills without dying" streaks.
 * The streak counter increases with each elimination and resets when the player is eliminated.
 * When the count reaches a value in ElimStreakTags, the matching tag is broadcast.
 *
 * Listen channel: "Elimination.Message" (FVerbMessage)
 * Emits:          tag from ElimStreakTags[N] (FVerbMessage) when streak length N is reached
 *
 * Migrated from UElimStreakProcessor (ShooterCore).
 * Subclass in Blueprint and fill ElimStreakTags to customise thresholds.
 */
UCLASS(Abstract)
class UElimStreakProcessor : public UGameplayMessageProcessor
{
	GENERATED_BODY()

public:

	virtual void StartListening() override;

protected:

	/**
	 * Map of streak length → broadcast tag.
	 * Example: { 3 → "Accolade.Streak3", 5 → "Accolade.Streak5" }
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Accolade|Streak")
	TMap<int32, FGameplayTag> ElimStreakTags;

private:

	void OnEliminationMessage(FGameplayTag Channel, const FVerbMessage& Payload);

	UPROPERTY(Transient)
	TMap<TObjectPtr<APlayerState>, int32> PlayerStreakHistory;
};
