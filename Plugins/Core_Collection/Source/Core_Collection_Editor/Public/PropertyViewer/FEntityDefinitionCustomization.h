#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "Templates/SharedPointer.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"

class IPropertyHandle;

class FEntityDefinitionCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
		class FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
		class IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:
	void RefreshEntityNames();

	TSharedPtr<IPropertyHandle> SelectedEntityHandle;
	TArray<TSharedPtr<FName>> AvailableNames;

	TSharedRef<SWidget> GenerateComboItem(TSharedPtr<FName> Item);
	void OnSelectionChanged(TSharedPtr<FName> NewValue, ESelectInfo::Type);
	FText GetCurrentSelection() const;
};
