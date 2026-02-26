#include "MessageProcessors/AssistProcessor.h"

#include "GameFramework/PlayerState.h"
#include "Messages/VerbMessageHelpers.h"
#include "Structs/VerbMessageTypes.h"
#include "NativeGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AssistProcessor)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Elimination_Message, "WarCollection.Elimination.Message");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Damage_Message,      "Core.Damage.Message");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Assist_Message,      "Assist.Message");

// ---------------------------------------------------------------------------

void UAssistProcessor::StartListening()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	AddListenerHandle(MessageSubsystem.RegisterListener(TAG_Elimination_Message, this, &ThisClass::OnEliminationMessage));
	AddListenerHandle(MessageSubsystem.RegisterListener(TAG_Damage_Message,      this, &ThisClass::OnDamageMessage));
}

// ---------------------------------------------------------------------------

void UAssistProcessor::OnDamageMessage(FGameplayTag Channel, const FVerbMessage& Payload)
{
	if (Payload.Instigator == Payload.Target) { return; }

	if (APlayerState* InstigatorPS = UVerbMessageHelpers::GetPlayerStateFromObject(Payload.Instigator))
	{
		if (APlayerState* TargetPS = UVerbMessageHelpers::GetPlayerStateFromObject(Payload.Target))
		{
			FPlayerAssistDamageTracking& Damage = DamageHistory.FindOrAdd(TargetPS);
			float& DamageTotalFromInstigator    = Damage.AccumulatedDamageByPlayer.FindOrAdd(InstigatorPS);
			DamageTotalFromInstigator += static_cast<float>(Payload.Magnitude);
		}
	}
}

// ---------------------------------------------------------------------------

void UAssistProcessor::OnEliminationMessage(FGameplayTag Channel, const FVerbMessage& Payload)
{
	if (APlayerState* TargetPS = Cast<APlayerState>(Payload.Target))
	{
		if (FPlayerAssistDamageTracking* DamageOnTarget = DamageHistory.Find(TargetPS))
		{
			UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);

			for (const auto& KVP : DamageOnTarget->AccumulatedDamageByPlayer)
			{
				if (APlayerState* AssistPS = KVP.Key)
				{
					// Do not grant an assist to the player who landed the kill
					if (AssistPS == Payload.Instigator) { continue; }

					FVerbMessage AssistMessage;
					AssistMessage.Verb         = TAG_Assist_Message;
					AssistMessage.Instigator   = AssistPS;
					AssistMessage.Target       = TargetPS;
					AssistMessage.TargetTags   = Payload.TargetTags;
					AssistMessage.ContextTags  = Payload.ContextTags;
					AssistMessage.Magnitude    = KVP.Value;

					MessageSubsystem.BroadcastMessage(AssistMessage.Verb, AssistMessage);
				}
			}

			DamageHistory.Remove(TargetPS);
		}
	}
}
