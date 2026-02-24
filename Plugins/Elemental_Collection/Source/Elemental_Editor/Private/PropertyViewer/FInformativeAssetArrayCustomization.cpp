// FInformativeAssetArrayCustomization.cpp

#include "PropertyViewer/FInformativeAssetArrayCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"    // für IPropertyTypeCustomizationUtils
#include "PropertyHandle.h"
#include "Widgets/SNullWidget.h"

#define LOCTEXT_NAMESPACE "FInformativeAssetArrayCustomization"

TSharedRef<IPropertyTypeCustomization> FInformativeAssetArrayCustomization::MakeInstance()
{
    return MakeShareable(new FInformativeAssetArrayCustomization());
}

void FInformativeAssetArrayCustomization::CustomizeHeader(
    TSharedRef<IPropertyHandle> ArrayPropertyHandle,
    FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& /*Unused*/)
{
    // Nur den Array-Namen anzeigen
    HeaderRow
        .NameContent()
        [
            ArrayPropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            SNullWidget::NullWidget
        ];
}

void FInformativeAssetArrayCustomization::CustomizeChildren(
    TSharedRef<IPropertyHandle> ArrayPropertyHandle,
    IDetailChildrenBuilder& ChildBuilder,
    IPropertyTypeCustomizationUtils& /*Unused*/)
{
    uint32 NumChildren = 0;
    ArrayPropertyHandle->GetNumChildren(NumChildren);

    for (uint32 Index = 0; Index < NumChildren; ++Index)
    {
        if (TSharedPtr<IPropertyHandle> ElementHandle = ArrayPropertyHandle->GetChildHandle(Index))
        {
            ChildBuilder.AddProperty(ElementHandle.ToSharedRef());
        }
    }
}

#undef LOCTEXT_NAMESPACE
