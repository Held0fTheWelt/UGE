#include "GameFeature_Interact_SettingsCustomization.h"
#include "GameFeature_Interact_Settings.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyHandle.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "FGameFeature_Interact_SettingsCustomization"

TSharedRef<IDetailCustomization> FGameFeature_Interact_SettingsCustomization::MakeInstance()
{
	return MakeShareable(new FGameFeature_Interact_SettingsCustomization);
}

void FGameFeature_Interact_SettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TSharedPtr<IPropertyHandle> UseGlobalQueryHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGameFeature_Interact_Settings, bUseGlobalQueryCollisionDebug));
	TSharedPtr<IPropertyHandle> GlobalDebugHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGameFeature_Interact_Settings, bGlobalQueryCollisionDebug));
	TSharedPtr<IPropertyHandle> DefinedDebugHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UGameFeature_Interact_Settings, DefinedCollisionDebug));

	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Interact");

	Category.AddProperty(UseGlobalQueryHandle.ToSharedRef());
	Category.AddProperty(GlobalDebugHandle.ToSharedRef());

	// Immer verstecken, um doppelte Anzeige zu verhindern
	DetailBuilder.HideProperty(DefinedDebugHandle);

	// Dynamisch anzeigen, aber vollständig eingebettet (nicht manuell via ValueContent)
	Category.AddProperty(DefinedDebugHandle.ToSharedRef())
		.Visibility(TAttribute<EVisibility>::Create(
			[UseGlobalQueryHandle, GlobalDebugHandle]()
			{
				bool bUseGlobal = false;
				bool bGlobal = true;

				UseGlobalQueryHandle->GetValue(bUseGlobal);
				GlobalDebugHandle->GetValue(bGlobal);

				return (bUseGlobal && !bGlobal) ? EVisibility::Visible : EVisibility::Collapsed;
			}));
}
