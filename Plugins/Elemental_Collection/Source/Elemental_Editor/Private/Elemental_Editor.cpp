#include "Elemental_Editor.h"

#include "PropertyViewer/FInformativeAssetClassRefCustomization.h"
#include "PropertyViewer/FViewTypeConfigurationCustomization.h"
#include "PropertyViewer/FDefiningContainerInformationCustomization.h"
#include "PropertyViewer/FActorAndComponentSelectorCustomization.h"
#include "PropertyViewer/FInformativeAssetArrayCustomization.h"
#include "Developer/Settings/Public/ISettingsModule.h"

DEFINE_LOG_CATEGORY(Elemental_Editor);

#define LOCTEXT_NAMESPACE "FElemental_Editor"

void FElemental_Editor::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomPropertyTypeLayout(
		"InformativeAssetClassRef",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FInformativeAssetClassRefCustomization::MakeInstance)
	);

	PropertyModule.RegisterCustomPropertyTypeLayout(
		"ViewTypeConfiguration",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FViewTypeConfigurationCustomization::MakeInstance)
	);

	PropertyModule.RegisterCustomPropertyTypeLayout(
		"ActorAndComponentSelector",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(
			&FActorAndComponentSelectorCustomization::MakeInstance
		)
	);

	PropertyModule.RegisterCustomPropertyTypeLayout(
		"DefiningContainerInformation",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(
			&FDefiningContainerInformationCustomization::MakeInstance
		)
	);

	//PropertyModule.RegisterCustomPropertyTypeLayout(
	//	"InformativeAssetArrayCustomization",
	//	FOnGetPropertyTypeCustomizationInstance::CreateStatic(
	//		&FInformativeAssetArrayCustomization::MakeInstance
	//	)
	//);


	PropertyModule.NotifyCustomizationModuleChanged();
	UE_LOG(Elemental_Editor, Log, TEXT("Elemental_Editor module has been loaded"));
}

void FElemental_Editor::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.UnregisterCustomPropertyTypeLayout("InformativeAssetClassRef");
	PropertyModule.UnregisterCustomPropertyTypeLayout("ViewTypeConfiguration");
	PropertyModule.UnregisterCustomPropertyTypeLayout("ActorAndComponentSelector");
	PropertyModule.UnregisterCustomPropertyTypeLayout("DefiningContainerInformation");
	//PropertyModule.UnregisterCustomPropertyTypeLayout("InformativeAssetArrayCustomization");
	PropertyModule.NotifyCustomizationModuleChanged();
	UE_LOG(Elemental_Editor, Log, TEXT("Elemental_Editor module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElemental_Editor, Elemental_Editor)