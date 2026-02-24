#include "Core_Setup.h"

#include "Core_Settings.h"
#include "GameInstance/CoreGameInstanceDelegates.h"
#include "ISettingsModule.h"
#include "Settings/CoreSettingsLocal.h"
#include "ISettingsContainer.h" // Add this include at the top of the file

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

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "War Collection", "Core Settings",
			LOCTEXT("RuntimeSettingsName", "Core Settings"), LOCTEXT("RuntimeSettingsDescription", "Core Setup for Classes"),
			GetMutableDefault<UCore_Settings>());
	}
	UE_LOG(Core_Setup, Log, TEXT("Core_Setup module has been loaded"));
}

void FCore_Setup::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "War Collection", "Core Settings");
	}

	UE_LOG(Core_Setup, Log, TEXT("Core_Setup module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCore_Setup, Core_Setup)