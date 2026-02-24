#include "AssetManager/AsyncAssetLoaderHelper.h"

DEFINE_LOG_CATEGORY_STATIC(Log_AsyncAssetLoaderHelper, Log, All);

void UAsyncAssetLoaderHelper::StartLoading(const TArray<FSoftObjectPath>& Paths)
{
	PendingPaths = Paths;
	RemainingLoads = 0;

	if (!StreamableManager.IsValid())
	{
		StreamableManager = MakeShared<FStreamableManager>();
	}

	// Schütze dieses Objekt vor Garbage Collection
	this->AddToRoot();

	BeginLoadingAll();
}

void UAsyncAssetLoaderHelper::BeginLoadingAll()
{
	for (const FSoftObjectPath& Path : PendingPaths)
	{
		if (!Path.IsValid())
		{
			UE_LOG(Log_AsyncAssetLoaderHelper, Warning, TEXT("Ungültiger Pfad übersprungen."));
			continue;
		}

		++RemainingLoads;

		StreamableManager->RequestAsyncLoad(
			Path,
			FStreamableDelegate::CreateUObject(this, &UAsyncAssetLoaderHelper::OnSingleAssetLoaded)
		);
	}

	// Falls kein Pfad gültig war
	if (RemainingLoads == 0)
	{
		OnAllAssetsLoaded.Broadcast();
		CleanupAfterLoad();
	}
}

void UAsyncAssetLoaderHelper::OnSingleAssetLoaded()
{
	--RemainingLoads;

	if (RemainingLoads <= 0)
	{
		OnAllAssetsLoaded.Broadcast();
		CleanupAfterLoad();
	}
}

void UAsyncAssetLoaderHelper::CleanupAfterLoad()
{
	// Jetzt kann GC dieses Objekt übernehmen
	this->RemoveFromRoot();

	// Optional: Logging
	UE_LOG(Log_AsyncAssetLoaderHelper, Verbose, TEXT("AsyncAssetLoaderHelper hat abgeschlossen und ist bereit für GC."));
}
