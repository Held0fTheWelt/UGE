// Copyright Epic Games, Inc. All Rights Reserved.

#include "Experience/UserFacingExperienceDefinition.h"
#include "CommonSessionSubsystem.h"
#include "Containers/UnrealString.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Interfaces/ReplaySupportProviderInterface.h"
#include "UObject/NameTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UserFacingExperienceDefinition)

UCommonSession_HostSessionRequest* UUserFacingExperienceDefinition::CreateHostingRequest(const UObject* WorldContextObject) const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();
	const FString UserFacingExperienceName = GetPrimaryAssetId().PrimaryAssetName.ToString();

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
	UCommonSession_HostSessionRequest* Result = nullptr;

	if (UCommonSessionSubsystem* Subsystem = GameInstance ? GameInstance->GetSubsystem<UCommonSessionSubsystem>() : nullptr)
	{
		Result = Subsystem->CreateOnlineHostSessionRequest();
	}

	if (!Result)
	{
		Result = NewObject<UCommonSession_HostSessionRequest>();
		Result->OnlineMode = ECommonSessionOnlineMode::Online;
		Result->bUseLobbies = true;
		Result->bUseLobbiesVoiceChat = false;
		Result->bUsePresence = !IsRunningDedicatedServer();
	}
	Result->MapID = MapID;
	Result->ModeNameForAdvertisement = UserFacingExperienceName;
	Result->ExtraArgs = ExtraArgs;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);
	Result->MaxPlayerCount = MaxPlayerCount;

	if (IReplaySupportProviderInterface* ReplayI = Cast<IReplaySupportProviderInterface>(GameInstance))
	{
		if (ReplayI->DoesPlatformSupportReplays() && bRecordReplay)
		{
			Result->ExtraArgs.Add(TEXT("DemoRec"), FString());
		}
	}

	return Result;
}
