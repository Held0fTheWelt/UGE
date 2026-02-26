// Copyright Epic Games, Inc. All Rights Reserved.

#include "CosmeticDeveloperSettings.h"
#include "CharacterPartTypes.h"
#include "ControllerComponent_CharacterParts.h"
#include "Interfaces/DevelopmentStaticsProviderInterface.h"
#include "Misc/App.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CosmeticDeveloperSettings)

#define LOCTEXT_NAMESPACE "CosmeticsCheats"

UCosmeticDeveloperSettings::UCosmeticDeveloperSettings()
{
}

FName UCosmeticDeveloperSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}

#if WITH_EDITOR

void UCosmeticDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ApplySettings();
}

void UCosmeticDeveloperSettings::PostReloadConfig(FProperty* PropertyThatWasLoaded)
{
	Super::PostReloadConfig(PropertyThatWasLoaded);

	ApplySettings();
}

void UCosmeticDeveloperSettings::PostInitProperties()
{
	Super::PostInitProperties();

	ApplySettings();
}

void UCosmeticDeveloperSettings::ApplySettings()
{
	if (GIsEditor && (GEngine != nullptr))
	{
		ReapplyLoadoutIfInPIE();
	}
}

void UCosmeticDeveloperSettings::ReapplyLoadoutIfInPIE()
{
#if WITH_SERVER_CODE
	if (IDevelopmentStaticsProvider* Provider = DevelopmentStaticsProviderRegistry::Get())
	{
		UWorld* ServerWorld = Provider->GetPlayInEditorAuthorityWorld();
		if (ServerWorld != nullptr)
		{
			ServerWorld->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([=]()
				{
					for (TActorIterator<APlayerController> PCIterator(ServerWorld); PCIterator; ++PCIterator)
					{
						if (APlayerController* PC = *PCIterator)
						{
							if (UControllerComponent_CharacterParts* CosmeticComponent = PC->FindComponentByClass<UControllerComponent_CharacterParts>())
							{
								CosmeticComponent->ApplyDeveloperSettings();
							}
						}
					}
				}));
		}
	}
#endif	// WITH_SERVER_CODE
}

void UCosmeticDeveloperSettings::OnPlayInEditorStarted() const
{
	// Show a notification toast to remind the user that there's an experience override set
	if (CheatCosmeticCharacterParts.Num() > 0)
	{
		FNotificationInfo Info(LOCTEXT("CosmeticOverrideActive", "Applying Cosmetic Override"));
		Info.ExpireDuration = 2.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
	}
}

#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
