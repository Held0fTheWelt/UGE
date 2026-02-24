#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class IPropertyHandle;
class IPropertyUtilities;

/** Interner Enum für Actor vs Component Mode */
enum class ECustomAssetDefinitionType : uint8
{
    None,
    Actor,
    Component
};

/**
 * Vereinfachte Customization: zeigt nur TargetClass + AssetDefinitions
 * je nach DefinitionType aus dem Parent-Struct.
 */
class FActorAndComponentSelectorCustomization : public IPropertyTypeCustomization
{
public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    // Nur Name, kein Value-Widget
    virtual void CustomizeHeader(
        TSharedRef<IPropertyHandle> StructPropertyHandle,
        class FDetailWidgetRow& HeaderRow,
        IPropertyTypeCustomizationUtils& CustomizationUtils
    ) override;

    // Hier bauen wir nur die Properties ein
    virtual void CustomizeChildren(
        TSharedRef<IPropertyHandle> StructPropertyHandle,
        class IDetailChildrenBuilder& StructBuilder,
        IPropertyTypeCustomizationUtils& CustomizationUtils
    ) override;

private:
    /** Für Refresh */
    TWeakPtr<IPropertyUtilities> PropertyUtilitiesPtr;

    /** Actor vs Component, aus Parent-Struct gelesen */
    ECustomAssetDefinitionType CachedDefinitionType = ECustomAssetDefinitionType::None;

    /** Wenn Parent DefinitionType sich ändert → UI neu aufbauen */
    void OnParentDefinitionChanged();
};
