#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StreamableManager.h"
#include "ControllerAsyncAssetLoaderHelper.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllInputAssetsLoaded, uint8, InputType);

/**
 * Lädt eine Liste von Objekten und feuert ein Event, sobald alle geladen sind.
 */
UCLASS(BlueprintType)
class ASSETLOADER_API UControllerAsyncAssetLoaderHelper : public UObject
{
	GENERATED_BODY()

public:
	/** Startet das Laden */
	void StartLoading(const TArray<FSoftObjectPath>& Paths, const uint8& InputType);

	/** Delegate: Wird aufgerufen, wenn alle Assets geladen sind */
	FOnAllInputAssetsLoaded OnAllAssetsLoaded;

protected:
	void BeginLoadingAll(const uint8& InputType);
	void OnSingleAssetLoaded(const uint8& InputType);

	/** Selbstzerstörung nach Abschluss */
	UFUNCTION()
	void CleanupAfterLoad();

private:
	TArray<FSoftObjectPath> PendingPaths;
	int32 RemainingLoads = 0;

	TSharedPtr<FStreamableManager> StreamableManager;
};
