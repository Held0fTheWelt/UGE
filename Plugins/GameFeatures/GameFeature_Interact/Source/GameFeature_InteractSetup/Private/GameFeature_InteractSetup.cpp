#include "GameFeature_InteractSetup.h"

#include "GameFeature_Interact_Settings.h"
#include "PropertyEditorModule.h"
#include "GameFeature_Interact_SettingsCustomization.h"
#include "ISettingsModule.h"

DEFINE_LOG_CATEGORY(GameFeature_InteractSetup);

#define LOCTEXT_NAMESPACE "FGameFeature_InteractSetup"

void FGameFeature_InteractSetup::StartupModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "War Collection", "Game Feature Interact Settings",
			LOCTEXT("RuntimeSettingsName", "Game Feature Interact Settings"), LOCTEXT("RuntimeSettingsDescription", "Game Feature Interact Setup"),
			GetMutableDefault<UGameFeature_Interact_Settings>());

	}

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout("GameFeature_Interact_Settings", FOnGetDetailCustomizationInstance::CreateStatic(&FGameFeature_Interact_SettingsCustomization::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();
	
	UE_LOG(GameFeature_InteractSetup, Log, TEXT("GameFeature_InteractSetup module has been loaded"));
}

void FGameFeature_InteractSetup::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "War Collection", "Game Feature Interact Settings");
	}
	
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout("GameFeature_Interact_Settings");
	}
	
	UE_LOG(GameFeature_InteractSetup, Log, TEXT("GameFeature_InteractSetup module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameFeature_InteractSetup, GameFeature_InteractSetup)