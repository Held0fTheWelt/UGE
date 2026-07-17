#include "Core_Setup.h"

#include "Core_Settings.h"
#include "GameInstance/CoreGameInstanceDelegates.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsContainer.h"
#endif
#include "Settings/CoreSettingsLocal.h"

DEFINE_LOG_CATEGORY(Core_Setup);

#define LOCTEXT_NAMESPACE "FCore_Setup"

void FCore_Setup::StartupModule()
{
	CoreGameInstanceDelegates::RegisterExperienceLoadedListener(FOnExperienceLoadedDelegate::CreateLambda([](const UObject*)
	{
		if (UCoreSettingsLocal* Settings = UCoreSettingsLocal::Get())
		{
			Settings->OnExperienceLoaded();
		}
	}));

#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "War Collection", "Core Settings",
			LOCTEXT("RuntimeSettingsName", "Core Settings"), LOCTEXT("RuntimeSettingsDescription", "Core Setup for Classes"),
			GetMutableDefault<UCore_Settings>());
	}
#endif
	UE_LOG(Core_Setup, Log, TEXT("Core_Setup module has been loaded"));
}

void FCore_Setup::ShutdownModule()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "War Collection", "Core Settings");
	}
#endif

	UE_LOG(Core_Setup, Log, TEXT("Core_Setup module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_Setup, Core_Setup)
