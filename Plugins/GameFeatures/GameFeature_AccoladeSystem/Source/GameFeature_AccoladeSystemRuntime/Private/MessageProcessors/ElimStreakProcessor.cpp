#include "MessageProcessors/ElimStreakProcessor.h"

#include "GameFramework/PlayerState.h"
#include "Structs/VerbMessageTypes.h"
#include "NativeGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ElimStreakProcessor)

namespace ElimStreak
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Elimination_Message, "WarCollection.Elimination.Message");
}

// ---------------------------------------------------------------------------

void UElimStreakProcessor::StartListening()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	AddListenerHandle(MessageSubsystem.RegisterListener(ElimStreak::TAG_Elimination_Message, this, &ThisClass::OnEliminationMessage));
}

// ---------------------------------------------------------------------------

void UElimStreakProcessor::OnEliminationMessage(FGameplayTag Channel, const FVerbMessage& Payload)
{
	// Increment streak for the attacker (skip self-eliminations)
	if (Payload.Instigator != Payload.Target)
	{
		if (APlayerState* InstigatorPS = Cast<APlayerState>(Payload.Instigator))
		{
			int32& StreakCount = PlayerStreakHistory.FindOrAdd(InstigatorPS);
			++StreakCount;

			if (FGameplayTag* pTag = ElimStreakTags.Find(StreakCount))
			{
				FVerbMessage StreakMessage;
				StreakMessage.Verb           = *pTag;
				StreakMessage.Instigator     = InstigatorPS;
				StreakMessage.InstigatorTags = Payload.InstigatorTags;
				StreakMessage.ContextTags    = Payload.ContextTags;
				StreakMessage.Magnitude      = StreakCount;

				UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
				MessageSubsystem.BroadcastMessage(StreakMessage.Verb, StreakMessage);
			}
		}
	}

	// Reset streak for the eliminated player
	if (APlayerState* TargetPS = Cast<APlayerState>(Payload.Target))
	{
		PlayerStreakHistory.Remove(TargetPS);
	}
}
