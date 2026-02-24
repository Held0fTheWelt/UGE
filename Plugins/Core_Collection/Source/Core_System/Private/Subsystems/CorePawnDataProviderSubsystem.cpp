// Copyright Epic Games, Inc. All Rights Reserved.

#include "Subsystems/CorePawnDataProviderSubsystem.h"
#include "AssetManager/AssetManager_Core.h"
#include "DataAssets/Primary/PawnData_Core.h"
#include "Interfaces/ExperienceManagerInterface.h"
#include "States/PlayerState_Elemental.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "AssetRegistry/AssetData.h"
#include "Subsystems/PawnDataProviderSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CorePawnDataProviderSubsystem)

void UCorePawnDataProviderSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UPawnDataProviderSubsystem::SetProviderForWorld(GetWorld(), this);
}

void UCorePawnDataProviderSubsystem::Deinitialize()
{
	UPawnDataProviderSubsystem::SetProviderForWorld(GetWorld(), nullptr);
	Super::Deinitialize();
}

UObject* UCorePawnDataProviderSubsystem::GetPawnDataForController(const AController* Controller) const
{
	if (Controller == nullptr) { return nullptr; }

	if (const APlayerState_Elemental* ElementalPS = Controller->GetPlayerState<APlayerState_Elemental>())
	{
		if (const UPawnData_Core* PawnData = ElementalPS->GetPawnData<UPawnData_Core>())
		{
			return const_cast<UPawnData_Core*>(PawnData);
		}
	}

	UWorld* World = GetWorld();
	if (!World || !World->GetGameState()) { return nullptr; }

	if (IExperienceManagerInterface* ExperienceInterface = Cast<IExperienceManagerInterface>(World->GetGameState()->FindComponentByInterface(UExperienceManagerInterface::StaticClass())))
	{
		if (ExperienceInterface->IsExperienceLoaded())
		{
			const FSoftObjectPath PawnDataPath = ExperienceInterface->GetDefaultPawnDataPathFromCurrentExperience();
			if (PawnDataPath.IsValid())
			{
				if (UObject* PawnData = GetPawnDataFromPath(PawnDataPath))
				{
					return PawnData;
				}
			}
			return GetDefaultPawnData();
		}
	}

	return nullptr;
}

UObject* UCorePawnDataProviderSubsystem::GetDefaultPawnData() const
{
	return const_cast<UPawnData_Core*>(UAssetManager_Core::Get().GetDefaultPawnData());
}

UObject* UCorePawnDataProviderSubsystem::GetPawnDataFromPath(const FSoftObjectPath& Path) const
{
	if (!Path.IsValid()) { return nullptr; }
	return UAssetManager_Core::Get().GetAsset(TSoftObjectPtr<UPawnData_Core>(Path));
}

bool UCorePawnDataProviderSubsystem::GetPrimaryAssetData(FPrimaryAssetId AssetId, FAssetData& OutAssetData) const
{
	return UAssetManager_Core::Get().GetPrimaryAssetData(AssetId, OutAssetData);
}
