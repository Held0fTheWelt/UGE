#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StreamableManager.h"
#include "AsyncAssetLoaderHelper.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllAssetsLoadedSimple);

/**
 * Lädt eine Liste von Objekten und feuert ein Event, sobald alle geladen sind.
 */
UCLASS(BlueprintType)
class ASSETLOADER_API UAsyncAssetLoaderHelper : public UObject
{
	GENERATED_BODY()

public:

	/** Startet das Laden */
	UFUNCTION(BlueprintCallable, Category = "AsyncLoading")
	void StartLoading(const TArray<FSoftObjectPath>& Paths);

	/** Delegate: Wird aufgerufen, wenn alle Assets geladen sind */
	UPROPERTY(BlueprintAssignable, Category = "AsyncLoading")
	FOnAllAssetsLoadedSimple OnAllAssetsLoaded;

protected:

	void BeginLoadingAll();
	void OnSingleAssetLoaded();

	/** Selbstzerstörung nach Abschluss */
	UFUNCTION()
	void CleanupAfterLoad();

private:
	TArray<FSoftObjectPath> PendingPaths;
	int32 RemainingLoads = 0;

	TSharedPtr<FStreamableManager> StreamableManager;
};
