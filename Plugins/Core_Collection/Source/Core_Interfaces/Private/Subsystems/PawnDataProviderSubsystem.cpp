// Copyright Epic Games, Inc. All Rights Reserved.

#include "Subsystems/PawnDataProviderSubsystem.h"
#include "Interfaces/PawnDataProviderInterface.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "AssetRegistry/AssetData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PawnDataProviderSubsystem)

namespace
{
	TMap<TWeakObjectPtr<UWorld>, TWeakObjectPtr<UObject>> GProviderByWorld;
}

void UPawnDataProviderSubsystem::SetProviderForWorld(UWorld* World, UObject* Provider)
{
	if (World)
	{
		const TWeakObjectPtr<UWorld> WorldKey(World);
		if (Provider)
		{
			GProviderByWorld.Add(WorldKey, Provider);
		}
		else
		{
			GProviderByWorld.Remove(WorldKey);
		}
	}
}

UObject* UPawnDataProviderSubsystem::GetProviderForWorld(UWorld* World)
{
	if (!World) { return nullptr; }
	const TWeakObjectPtr<UObject>* Ptr = GProviderByWorld.Find(TWeakObjectPtr<UWorld>(World));
	return Ptr ? Ptr->Get() : nullptr;
}

UObject* UPawnDataProviderSubsystem::GetPawnDataForController(const AController* Controller) const
{
	if (IPawnDataProviderInterface* Provider = Cast<IPawnDataProviderInterface>(GetProviderForWorld(GetWorld())))
	{
		return Provider->GetPawnDataForController(Controller);
	}
	return nullptr;
}

UObject* UPawnDataProviderSubsystem::GetDefaultPawnData() const
{
	if (IPawnDataProviderInterface* Provider = Cast<IPawnDataProviderInterface>(GetProviderForWorld(GetWorld())))
	{
		return Provider->GetDefaultPawnData();
	}
	return nullptr;
}

UObject* UPawnDataProviderSubsystem::GetPawnDataFromPath(const FSoftObjectPath& Path) const
{
	if (IPawnDataProviderInterface* Provider = Cast<IPawnDataProviderInterface>(GetProviderForWorld(GetWorld())))
	{
		return Provider->GetPawnDataFromPath(Path);
	}
	return nullptr;
}

bool UPawnDataProviderSubsystem::GetPrimaryAssetData(FPrimaryAssetId AssetId, FAssetData& OutAssetData) const
{
	if (IPawnDataProviderInterface* Provider = Cast<IPawnDataProviderInterface>(GetProviderForWorld(GetWorld())))
	{
		return Provider->GetPrimaryAssetData(AssetId, OutAssetData);
	}
	return false;
}
