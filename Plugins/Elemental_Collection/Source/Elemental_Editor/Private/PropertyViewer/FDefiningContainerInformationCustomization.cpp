#include "PropertyViewer/FDefiningContainerInformationCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "PropertyHandle.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SNullWidget.h"

#include "Structs/DefiningContainerInformation.h"  // DefinitionType, DefiningAssetName, TargetClasses

#define LOCTEXT_NAMESPACE "FDefiningContainerInformationCustomization"

TSharedRef<IPropertyTypeCustomization> FDefiningContainerInformationCustomization::MakeInstance()
{
    return MakeShareable(new FDefiningContainerInformationCustomization());
}

void FDefiningContainerInformationCustomization::CustomizeHeader(
    TSharedRef<IPropertyHandle> StructPropertyHandle,
    FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    // Für möglichen Refresh cachen
    PropertyUtilities = CustomizationUtils.GetPropertyUtilities();

    // Versuche, den 'DefiningAssetName'-Handle zu kriegen
    TSharedPtr<IPropertyHandle> NameHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(
            FDefiningContainerInformation, DefiningAssetName));

    // Standard-Anzeige-Name aus dem Property
    FText DisplayName = StructPropertyHandle->GetPropertyDisplayName();

    // Wenn NameHandle gültig ist und ein Name drinnen steht, nutze den
    if (NameHandle.IsValid())
    {
        FName FoundName;
        if (NameHandle->GetValue(FoundName) == FPropertyAccess::Success
            && !FoundName.IsNone())
        {
            DisplayName = FText::FromName(FoundName);
        }

        // Falls du auf Änderungen von DefiningAssetName direkt reagieren willst:
        NameHandle->SetOnPropertyValueChanged(
            FSimpleDelegate::CreateSP(this, &FDefiningContainerInformationCustomization::OnParentPropertyChanged)
        );
    }

    // Baue den Header
    HeaderRow
        .NameContent()
        [
            SNew(STextBlock)
                .Text(DisplayName)
                // optional: eigene Font-Choice hier
        ]
        .ValueContent()
        [
            SNullWidget::NullWidget
        ];
}

void FDefiningContainerInformationCustomization::CustomizeChildren(
    TSharedRef<IPropertyHandle> StructPropertyHandle,
    IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& /*CustomizationUtils*/)
{
    // Einfach alle Properties in der Reihenfolge ihrer Deklaration auflisten
    uint32 NumChildren = 0;
    if (StructPropertyHandle->GetNumChildren(NumChildren) == FPropertyAccess::Success)
    {
        for (uint32 Index = 0; Index < NumChildren; ++Index)
        {
            if (TSharedPtr<IPropertyHandle> ChildHandle =
                StructPropertyHandle->GetChildHandle(Index))
            {
                StructBuilder.AddProperty(ChildHandle.ToSharedRef());
            }
        }
    }
}

void FDefiningContainerInformationCustomization::OnParentPropertyChanged()
{
    if (TSharedPtr<IPropertyUtilities> Utils = PropertyUtilities.Pin())
    {
        Utils->RequestRefresh();
    }
}

#undef LOCTEXT_NAMESPACE
