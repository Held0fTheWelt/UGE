#include "PropertyViewer/FViewTypeConfigurationCustomization.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Structs/ViewTypeConfiguration.h"
#include "Widgets/Text/STextBlock.h"
#include "PropertyCustomizationHelpers.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "FViewTypeConfigurationCustomization"

TSharedRef<IPropertyTypeCustomization> FViewTypeConfigurationCustomization::MakeInstance()
{
    return MakeShareable(new FViewTypeConfigurationCustomization);
}

void FViewTypeConfigurationCustomization::CustomizeHeader(
    TSharedRef<IPropertyHandle> StructPropertyHandle,
    FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    // Get the child handle for the enum ViewType
    TSharedPtr<IPropertyHandle> ViewTypeHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FViewTypeConfiguration, ViewType));

    // Retrieve the display text for the enum value
    FText EnumDisplayName = LOCTEXT("InvalidViewType", "Invalid");
    if (ViewTypeHandle.IsValid() && ViewTypeHandle->GetValueAsDisplayText(EnumDisplayName) != FPropertyAccess::Fail)
    {
        // Got valid display name
    }

    HeaderRow
        .NameContent()
        [
            StructPropertyHandle->CreatePropertyNameWidget()  // still show property name or blank
        ]
        .ValueContent()
        [
            SNew(STextBlock)
                .Text(EnumDisplayName)
                .Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
        ];
}

void FViewTypeConfigurationCustomization::CustomizeChildren(
    TSharedRef<IPropertyHandle> StructPropertyHandle,
    IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    // Show all children (ViewType, plus any other fields) under the custom header
    uint32 NumChildren;
    StructPropertyHandle->GetNumChildren(NumChildren);
    for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
    {
        TSharedPtr<IPropertyHandle> ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex);
        if (ChildHandle.IsValid())
        {
            StructBuilder.AddProperty(ChildHandle.ToSharedRef());
        }
    }
}

#undef LOCTEXT_NAMESPACE
