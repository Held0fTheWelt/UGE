#include "Accolades/AccoladeHostWidget.h"

#include "DataRegistrySubsystem.h"
#include "Messages/NotificationMessage.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AccoladeHostWidget)

class UUserWidget;

// The channel tag that accolade notification messages use.
// Broadcasters must set FNotificationMessage::TargetChannel to this value.
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Accolade_Message, "Accolade.Message");

// DataRegistry source name for accolade rows
static FName NAME_AccoladeRegistryID("Accolades");

// ---------------------------------------------------------------------------

void UAccoladeHostWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	ListenerHandle = MessageSubsystem.RegisterListener(TAG_AddNotification_Message, this, &ThisClass::OnNotificationMessage);
}

void UAccoladeHostWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(ListenerHandle);

	CancelAsyncLoading();

	Super::NativeDestruct();
}

// ---------------------------------------------------------------------------

void UAccoladeHostWidget::OnNotificationMessage(FGameplayTag Channel, const FNotificationMessage& Notification)
{
	if (Notification.TargetChannel != TAG_Accolade_Message)
	{
		return;
	}

	// If the notification targets a specific player, ignore it when it isn't ours
	if (Notification.TargetPlayer != nullptr)
	{
		APlayerController* PC = GetOwningPlayer();
		if (!PC || PC->PlayerState != Notification.TargetPlayer)
		{
			return;
		}
	}

	// Acquire the data registry row for this accolade tag
	const int32 NextID = AllocatedSequenceID;
	++AllocatedSequenceID;

	const FDataRegistryId ItemID(NAME_AccoladeRegistryID, Notification.PayloadTag.GetTagName());
	if (!UDataRegistrySubsystem::Get()->AcquireItem(
		ItemID,
		FDataRegistryItemAcquiredCallback::CreateUObject(this, &ThisClass::OnRegistryLoadCompleted, NextID)))
	{
		UE_LOG(LogTemp, Error,
			TEXT("UAccoladeHostWidget: Failed to find accolade registry entry for tag '%s'. Accolade will not appear."),
			*Notification.PayloadTag.GetTagName().ToString());
		--AllocatedSequenceID;
	}
}

// ---------------------------------------------------------------------------

void UAccoladeHostWidget::OnRegistryLoadCompleted(const FDataRegistryAcquireResult& AccoladeHandle, int32 SequenceID)
{
	if (const FAccoladeDefinitionRow* AccoladeRow = AccoladeHandle.GetItem<FAccoladeDefinitionRow>())
	{
		FPendingAccoladeEntry& PendingEntry = PendingAccoladeLoads.AddDefaulted_GetRef();
		PendingEntry.Row        = *AccoladeRow;
		PendingEntry.SequenceID = SequenceID;

		TArray<FSoftObjectPath> AssetsToLoad;
		AssetsToLoad.Add(AccoladeRow->Sound.ToSoftObjectPath());
		AssetsToLoad.Add(AccoladeRow->Icon.ToSoftObjectPath());

		AsyncLoad(AssetsToLoad, [this, SequenceID]
		{
			FPendingAccoladeEntry* EntryThatFinishedLoading = PendingAccoladeLoads.FindByPredicate(
				[SequenceID](const FPendingAccoladeEntry& Entry) { return Entry.SequenceID == SequenceID; });

			if (ensure(EntryThatFinishedLoading))
			{
				EntryThatFinishedLoading->Sound             = EntryThatFinishedLoading->Row.Sound.Get();
				EntryThatFinishedLoading->Icon              = EntryThatFinishedLoading->Row.Icon.Get();
				EntryThatFinishedLoading->bFinishedLoading  = true;
				ConsiderLoadedAccolades();
			}
		});

		StartAsyncLoading();
	}
	else
	{
		ensure(false);
	}
}

// ---------------------------------------------------------------------------

void UAccoladeHostWidget::ConsiderLoadedAccolades()
{
	int32 PendingIndexToDisplay;
	do
	{
		PendingIndexToDisplay = PendingAccoladeLoads.IndexOfByPredicate(
			[DesiredID = NextDisplaySequenceID](const FPendingAccoladeEntry& Entry)
			{
				return Entry.bFinishedLoading && Entry.SequenceID == DesiredID;
			});

		if (PendingIndexToDisplay != INDEX_NONE)
		{
			FPendingAccoladeEntry Entry = MoveTemp(PendingAccoladeLoads[PendingIndexToDisplay]);
			PendingAccoladeLoads.RemoveAtSwap(PendingIndexToDisplay);

			ProcessLoadedAccolade(Entry);
			++NextDisplaySequenceID;
		}
	}
	while (PendingIndexToDisplay != INDEX_NONE);
}

// ---------------------------------------------------------------------------

void UAccoladeHostWidget::ProcessLoadedAccolade(const FPendingAccoladeEntry& Entry)
{
	// This widget instance only shows accolades for its own location slot
	if (Entry.Row.LocationTag != LocationName)
	{
		return;
	}

	bool bRecreateWidget = (PendingAccoladeDisplays.Num() == 0);

	// Cancel any pending accolades that this new one supersedes
	for (int32 Index = 0; Index < PendingAccoladeDisplays.Num(); )
	{
		if (PendingAccoladeDisplays[Index].Row.AccoladeTags.HasAny(Entry.Row.CancelAccoladesWithTag))
		{
			if (UUserWidget* OldWidget = PendingAccoladeDisplays[Index].AllocatedWidget)
			{
				DestroyAccoladeWidget(OldWidget);
				bRecreateWidget = true;
			}
			PendingAccoladeDisplays.RemoveAt(Index);
		}
		else
		{
			++Index;
		}
	}

	PendingAccoladeDisplays.Add(Entry);

	if (bRecreateWidget)
	{
		DisplayNextAccolade();
	}
}

// ---------------------------------------------------------------------------

void UAccoladeHostWidget::DisplayNextAccolade()
{
	if (PendingAccoladeDisplays.Num() > 0)
	{
		FPendingAccoladeEntry& Entry = PendingAccoladeDisplays[0];
		GetWorld()->GetTimerManager().SetTimer(
			NextTimeToReconsiderHandle,
			this, &ThisClass::PopDisplayedAccolade,
			Entry.Row.DisplayDuration);

		Entry.AllocatedWidget = CreateAccoladeWidget(Entry);
	}
}

// ---------------------------------------------------------------------------

void UAccoladeHostWidget::PopDisplayedAccolade()
{
	if (PendingAccoladeDisplays.Num() > 0)
	{
		if (UUserWidget* OldWidget = PendingAccoladeDisplays[0].AllocatedWidget)
		{
			DestroyAccoladeWidget(OldWidget);
		}
		PendingAccoladeDisplays.RemoveAt(0);
	}

	DisplayNextAccolade();
}
