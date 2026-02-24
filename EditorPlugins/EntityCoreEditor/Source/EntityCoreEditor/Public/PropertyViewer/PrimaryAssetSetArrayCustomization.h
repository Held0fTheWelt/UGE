#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class IPropertyHandle;
class IPropertyUtilities;

class FPrimaryAssetSetArrayCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// IPropertyTypeCustomization overrides
	virtual void CustomizeHeader(
		TSharedRef<IPropertyHandle> StructPropertyHandle,
		class FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeChildren(
		TSharedRef<IPropertyHandle> StructPropertyHandle,
		class IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;

private:
	// Aktualisiert die Liste der Asset-Namen
	void UpdateAssetNames(TSharedPtr<IPropertyHandle> ArrayHandle);

	// Handler, um Änderungen am Array zu erfassen
	void HandleArrayChanged();

	// Handler, um das Delegate abzufangen und den View zu aktualisieren
	void HandleAssetsChanged();

private:
	// Gespeicherte Asset-Namen für die ComboBox
	TArray<TSharedPtr<FString>> AssetNames;

	// Handles zum Zugriff auf Properties
	TSharedPtr<IPropertyHandle> ArrayHandle;
	TSharedPtr<IPropertyHandle> IndexHandle;

	// Für das Property-Refresh
	TSharedPtr<IPropertyUtilities> PropUtils;
};
