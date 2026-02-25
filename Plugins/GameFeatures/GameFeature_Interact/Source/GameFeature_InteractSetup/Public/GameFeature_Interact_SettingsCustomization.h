#pragma once

#include "IDetailCustomization.h"

class IDetailLayoutBuilder;
class IPropertyHandle;

class FGameFeature_Interact_SettingsCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};