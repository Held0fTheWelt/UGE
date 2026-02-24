// FInformativeAssetArrayCustomization.h

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class IPropertyHandle;
class IPropertyUtilities;
class IDetailChildrenBuilder;

/**
 * Property‐Customization für TArray<FInformativeAssetClassRef>
 * (Deine AssetDefinitions – zeigt pro Element z.B. einen ClassViewer o.ä.)
 */
class FInformativeAssetArrayCustomization : public IPropertyTypeCustomization
{
public:
    /** Factory‐Methode, um den Customizer zu registrieren */
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    /** Header: hier kannst Du z.B. nur den Array‐Namen anzeigen oder eine "Add Element" Schaltfläche */
    virtual void CustomizeHeader(
        TSharedRef<IPropertyHandle> ArrayPropertyHandle,
        class FDetailWidgetRow& HeaderRow,
        IPropertyTypeCustomizationUtils& CustomizationUtils
    ) override;

    /** Children: hier baust Du für jedes Array‐Element das UI (ClassPicker, Labels, Remove‐Button etc.) */
    virtual void CustomizeChildren(
        TSharedRef<IPropertyHandle> ArrayPropertyHandle,
        IDetailChildrenBuilder& ChildBuilder,
        IPropertyTypeCustomizationUtils& CustomizationUtils
    ) override;

private:
    /** Für Refresh/Undo‐Redo */
    TWeakPtr<IPropertyUtilities> PropertyUtilitiesPtr;

    /** Callback, wenn sich das Array verändert (Element hinzugefügt/entfernt) */
    void OnArrayChanged();

    /** Hilfsfunktion zum Erzeugen des UI für ein einzelnes Asset‐Element */
    void AddAssetElementRow(
        IDetailChildrenBuilder& ChildBuilder,
        TSharedRef<IPropertyHandle> ElementHandle
    );
};
