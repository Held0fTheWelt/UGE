#include "PropertyViewer/FActorAndComponentSelectorCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "PropertyHandle.h"
#include "Widgets/SNullWidget.h"

#include "Structs/ActorAndComponentSelector.h"
#include "Structs/DefiningContainerInformation.h"

#define LOCTEXT_NAMESPACE "FActorAndComponentSelectorCustomization"

TSharedRef<IPropertyTypeCustomization> FActorAndComponentSelectorCustomization::MakeInstance()
{
    return MakeShareable(new FActorAndComponentSelectorCustomization());
}

void FActorAndComponentSelectorCustomization::CustomizeHeader(
    TSharedRef<IPropertyHandle> StructPropertyHandle,
    FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& /*Unused*/)
{
    // Nur der Name soll angezeigt werden
    HeaderRow
        .NameContent()
        [StructPropertyHandle->CreatePropertyNameWidget()]
        .ValueContent()
        [SNullWidget::NullWidget];
}

void FActorAndComponentSelectorCustomization::CustomizeChildren(
    TSharedRef<IPropertyHandle> StructPropertyHandle,
    IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    // Utilities fürs spätere Refresh merken
    PropertyUtilitiesPtr = StructCustomizationUtils.GetPropertyUtilities();

    // 1) DefinitionType aus dem Parent-Struct lesen
    CachedDefinitionType = ECustomAssetDefinitionType::None;
    if (TSharedPtr<IPropertyHandle> Parent = StructPropertyHandle->GetParentHandle())
    {
        if (TSharedPtr<IPropertyHandle> Grand = Parent->GetParentHandle())
        {
            if (TSharedPtr<IPropertyHandle> DefHandle =
                Grand->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDefiningContainerInformation, DefinitionType)))
            {
                // Änderungs-Callback registrieren
                DefHandle->SetOnPropertyValueChanged(
                    FSimpleDelegate::CreateSP(this, &FActorAndComponentSelectorCustomization::OnParentDefinitionChanged)
                );

                // tatsächlichen Wert holen
                uint8 Raw = 0;
                if (DefHandle->GetValue(Raw) == FPropertyAccess::Success)
                {
                    if (Raw == uint8(EAssetDefinitionType::ADT_ACTOR))     CachedDefinitionType = ECustomAssetDefinitionType::Actor;
                    else if (Raw == uint8(EAssetDefinitionType::ADT_COMPONENT)) CachedDefinitionType = ECustomAssetDefinitionType::Component;
                }
            }
        }
    }

    // 2) Handles für unsere beiden UPROPERTYs
    TSharedRef<IPropertyHandle> ClassHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FActorAndComponentSelector, TargetClass)).ToSharedRef();
    TSharedRef<IPropertyHandle> AssetDefsHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FActorAndComponentSelector, AssetDefinitions)).ToSharedRef();

    // 3) Wenn Actor/Component-Modus, zeigen wir nur diese beiden Felder
    if (CachedDefinitionType != ECustomAssetDefinitionType::None)
    {
        StructBuilder.AddProperty(ClassHandle);
        StructBuilder.AddProperty(AssetDefsHandle);
        return;
    }

    // 4) Fallback: bei allen anderen Fällen alle Felder normal anzeigen
    uint32 NumChildren = 0;
    StructPropertyHandle->GetNumChildren(NumChildren);
    for (uint32 Index = 0; Index < NumChildren; ++Index)
    {
        if (TSharedPtr<IPropertyHandle> Child = StructPropertyHandle->GetChildHandle(Index))
        {
            StructBuilder.AddProperty(Child.ToSharedRef());
        }
    }
}

void FActorAndComponentSelectorCustomization::OnParentDefinitionChanged()
{
    if (TSharedPtr<IPropertyUtilities> Utils = PropertyUtilitiesPtr.Pin())
    {
        // Ganze Details-Panel neu bauen
        Utils->RequestRefresh();
    }
}

#undef LOCTEXT_NAMESPACE
