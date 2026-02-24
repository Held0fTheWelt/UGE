#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class IPropertyHandle;
class IPropertyUtilities;
class IDetailChildrenBuilder;

/**
 * Customization für FDefiningContainerInformation:
 * - Header zeigt FName aus DefiningAssetName wenn gesetzt
 * - Children listet alle Properties in Standard-Reihenfolge
 */
class FDefiningContainerInformationCustomization : public IPropertyTypeCustomization
{
public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    // Header: Label = DefiningAssetName (oder Property-Name), kein Value-Widget
    virtual void CustomizeHeader(
        TSharedRef<IPropertyHandle> StructPropertyHandle,
        class FDetailWidgetRow& HeaderRow,
        IPropertyTypeCustomizationUtils& CustomizationUtils
    ) override;

    // Children: einfach alle Child-Properties adden
    virtual void CustomizeChildren(
        TSharedRef<IPropertyHandle> StructPropertyHandle,
        class IDetailChildrenBuilder& StructBuilder,
        IPropertyTypeCustomizationUtils& CustomizationUtils
    ) override;

private:
    // zum Refresh, falls DefinitionType (oder sonstwas) sich ändern sollte
    TWeakPtr<IPropertyUtilities> PropertyUtilities;

    // kann genutzt werden, um auf Änderungen des DefinitionType zu reagieren
    void OnParentPropertyChanged();
};
