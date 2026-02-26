#include "MessageProcessors/ElimChainProcessor.h"

#include "GameFramework/PlayerState.h"
#include "Structs/VerbMessageTypes.h"
#include "NativeGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ElimChainProcessor)

namespace ElimChain
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Elimination_Message, "WarCollection.Elimination.Message");
}

// ---------------------------------------------------------------------------

void UElimChainProcessor::StartListening()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	AddListenerHandle(MessageSubsystem.RegisterListener(ElimChain::TAG_Elimination_Message, this, &ThisClass::OnEliminationMessage));
}

// ---------------------------------------------------------------------------

void UElimChainProcessor::OnEliminationMessage(FGameplayTag Channel, const FVerbMessage& Payload)
{
	// Self-eliminations do not count towards a chain
	if (Payload.Instigator == Payload.Target) { return; }

	if (APlayerState* InstigatorPS = Cast<APlayerState>(Payload.Instigator))
	{
		const double CurrentTime = GetServerTime();

		FPlayerElimChainInfo& History = PlayerChainHistory.FindOrAdd(InstigatorPS);
		const bool bChainExpired = (History.LastEliminationTime == 0.0)
			|| (History.LastEliminationTime + ChainTimeLimit < CurrentTime);

		History.LastEliminationTime = CurrentTime;

		if (bChainExpired)
		{
			History.ChainCounter = 1;
		}
		else
		{
			++History.ChainCounter;

			if (FGameplayTag* pTag = ElimChainTags.Find(History.ChainCounter))
			{
				FVerbMessage ChainMessage;
				ChainMessage.Verb           = *pTag;
				ChainMessage.Instigator     = InstigatorPS;
				ChainMessage.InstigatorTags = Payload.InstigatorTags;
				ChainMessage.ContextTags    = Payload.ContextTags;
				ChainMessage.Magnitude      = History.ChainCounter;

				UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
				MessageSubsystem.BroadcastMessage(ChainMessage.Verb, ChainMessage);
			}
		}
	}
}
