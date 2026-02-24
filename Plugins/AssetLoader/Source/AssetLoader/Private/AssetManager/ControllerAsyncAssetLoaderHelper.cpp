#include "AssetManager/ControllerAsyncAssetLoaderHelper.h"

DEFINE_LOG_CATEGORY_STATIC(Log_AsyncInputAssetLoaderHelper, Log, All);

void UControllerAsyncAssetLoaderHelper::StartLoading(const TArray<FSoftObjectPath>& Paths, const uint8& InputType)
{
	PendingPaths = Paths;
	RemainingLoads = 0;

	if (!StreamableManager.IsValid())
	{
		StreamableManager = MakeShared<FStreamableManager>();
	}

	// Schütze dieses Objekt vor Garbage Collection
	this->AddToRoot();

	BeginLoadingAll(InputType);
}

void UControllerAsyncAssetLoaderHelper::BeginLoadingAll(const uint8& InputType)
{
	for (const FSoftObjectPath& Path : PendingPaths)
	{
		if (!Path.IsValid())
		{
			UE_LOG(Log_AsyncInputAssetLoaderHelper, Warning, TEXT("Ungültiger Pfad übersprungen."));
			continue;
		}

		++RemainingLoads;

		StreamableManager->RequestAsyncLoad(
			Path,
			FStreamableDelegate::CreateLambda([this, InputType]()
				{
					this->OnSingleAssetLoaded(InputType);
				})
		);
	}

	// Falls kein Pfad gültig war
	if (RemainingLoads == 0)
	{
		OnAllAssetsLoaded.Broadcast(InputType);
		CleanupAfterLoad();
	}
}

void UControllerAsyncAssetLoaderHelper::OnSingleAssetLoaded(const uint8& InputType)
{
	--RemainingLoads;

	if (RemainingLoads <= 0)
	{
		OnAllAssetsLoaded.Broadcast(InputType);
		CleanupAfterLoad();
	}
}

void UControllerAsyncAssetLoaderHelper::CleanupAfterLoad()
{
	// Jetzt kann GC dieses Objekt übernehmen
	this->RemoveFromRoot();

	// Optional: Logging
	UE_LOG(Log_AsyncInputAssetLoaderHelper, Verbose, TEXT("AsyncAssetLoaderHelper hat abgeschlossen und ist bereit für GC."));
}
