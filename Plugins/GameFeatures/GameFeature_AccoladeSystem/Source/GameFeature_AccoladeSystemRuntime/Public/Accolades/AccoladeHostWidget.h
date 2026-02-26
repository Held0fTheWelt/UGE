#pragma once

#include "Accolades/AccoladeDefinition.h"
#include "AsyncMixin.h"
#include "CommonUserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"

#include "AccoladeHostWidget.generated.h"

class USoundBase;
class UObject;
class UUserWidget;
struct FDataRegistryAcquireResult;
struct FNotificationMessage;

/**
 * FPendingAccoladeEntry
 *
 * Tracks one in-flight accolade: async-loading state, the loaded row data,
 * and the allocated display widget (once shown).
 */
USTRUCT(BlueprintType)
struct FPendingAccoladeEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FAccoladeDefinitionRow Row;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USoundBase> Sound = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> Icon = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> AllocatedWidget = nullptr;

	int32 SequenceID = 0;

	bool bFinishedLoading = false;
};

/**
 * UAccoladeHostWidget
 *
 * A CommonUI widget that hosts the accolade (kill/streak) toast queue.
 *
 * Workflow:
 *  1. Listens to TAG_AddNotification_Message (global channel).
 *  2. Filters: TargetChannel must equal TAG_Accolade_Message (module-local tag).
 *  3. Also filters: FAccoladeDefinitionRow::LocationTag must match LocationName.
 *  4. Async-loads Sound + Icon from DataRegistry, then calls CreateAccoladeWidget.
 *  5. Cancels any pending accolades whose tags overlap CancelAccoladesWithTag.
 *
 * Migrated from ULyraAccoladeHostWidget (ShooterCore).
 * FLyraNotificationMessage → FNotificationMessage (Elemental_MessageSystem).
 */
UCLASS(BlueprintType)
class UAccoladeHostWidget : public UCommonUserWidget, public FAsyncMixin
{
	GENERATED_BODY()

public:

	/**
	 * Only accolades whose LocationTag matches this value are displayed by this widget instance.
	 * Allows multiple host widgets on screen (e.g., top-left vs. center).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Accolade")
	FGameplayTag LocationName;

	//~UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~End of UUserWidget interface

	/** Called when an accolade widget should be removed (Blueprint implements cleanup/animation). */
	UFUNCTION(BlueprintImplementableEvent)
	void DestroyAccoladeWidget(UUserWidget* Widget);

	/** Called when a new accolade should be shown (Blueprint creates and returns the widget). */
	UFUNCTION(BlueprintImplementableEvent)
	UUserWidget* CreateAccoladeWidget(const FPendingAccoladeEntry& Entry);

private:

	FGameplayMessageListenerHandle ListenerHandle;

	int32 NextDisplaySequenceID = 0;
	int32 AllocatedSequenceID   = 0;

	FTimerHandle NextTimeToReconsiderHandle;

	/** Async loads in progress (may arrive out of order). */
	UPROPERTY(Transient)
	TArray<FPendingAccoladeEntry> PendingAccoladeLoads;

	/** Queued-up accolades waiting for display (first = currently visible). */
	UPROPERTY(Transient)
	TArray<FPendingAccoladeEntry> PendingAccoladeDisplays;

	void OnNotificationMessage(FGameplayTag Channel, const FNotificationMessage& Notification);
	void OnRegistryLoadCompleted(const FDataRegistryAcquireResult& AccoladeHandle, int32 SequenceID);

	void ConsiderLoadedAccolades();
	void PopDisplayedAccolade();
	void ProcessLoadedAccolade(const FPendingAccoladeEntry& Entry);
	void DisplayNextAccolade();
};
