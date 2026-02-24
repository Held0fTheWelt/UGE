// FInformativeAssetClassRefCustomization.cpp

#include "PropertyViewer/FInformativeAssetClassRefCustomization.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "DataAssets/Primary/InformativePrimaryDataAsset.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SComboBox.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "FInformativeAssetClassRefCustomization"

// No-op deleter for TSharedPtr<UClass>
struct FNoopDeleter
{
    void operator()(UClass* /*Unused*/) const {}
};

TSharedRef<IPropertyTypeCustomization> FInformativeAssetClassRefCustomization::MakeInstance()
{
    return MakeShareable(new FInformativeAssetClassRefCustomization);
}

void FInformativeAssetClassRefCustomization::CustomizeHeader(
    TSharedRef<IPropertyHandle> StructPropertyHandle,
    FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    // Store handle for lambdas
    ClassHandle = StructPropertyHandle->GetChildHandle(
        GET_MEMBER_NAME_CHECKED(FInformativeAssetClassRef, AssetClass));

    // Populate list of valid classes, filtering duplicates by AssetTypeName
    TMap<FName, TSharedPtr<UClass>> UniqueByType;
    for (TObjectIterator<UClass> It; It; ++It)
    {
        UClass* Cls = *It;
        if (Cls->HasAnyClassFlags(CLASS_Abstract) ||
            !Cls->IsChildOf(UInformativePrimaryDataAsset::StaticClass()) ||
            Cls == UInformativePrimaryDataAsset::StaticClass())
        {
            continue;
        }

        if (UInformativePrimaryDataAsset* DefaultObj =
            Cls->GetDefaultObject<UInformativePrimaryDataAsset>())
        {
            FName TypeName = DefaultObj->AssetTypeName;
            TSharedPtr<UClass> NewPtr = MakeShareable(Cls, FNoopDeleter());

            if (TSharedPtr<UClass>* ExistingPtr = UniqueByType.Find(TypeName))
            {
                UClass* ExistingClass = (*ExistingPtr).Get();
                // If the new class is a parent of the existing, replace
                if (ExistingClass->IsChildOf(Cls))
                {
                    UniqueByType[TypeName] = NewPtr;
                }
            }
            else
            {
                UniqueByType.Add(TypeName, NewPtr);
            }
        }
    }

    ValidClasses.Empty();
    for (auto& Pair : UniqueByType)
    {
        ValidClasses.Add(Pair.Value);
    }

    // Determine initial selection from property
    SelectedClass.Reset();
    {
        FString CurrentValue;
        if (ClassHandle->GetValueAsFormattedString(CurrentValue) == FPropertyAccess::Success)
        {
            UClass* Loaded = LoadClass<UObject>(nullptr, *CurrentValue);
            for (auto& Candidate : ValidClasses)
            {
                if (Candidate.IsValid() && Candidate.Get() == Loaded)
                {
                    SelectedClass = Candidate;
                    break;
                }
            }
        }
    }

    // Build the custom widget
    HeaderRow
        .NameContent()
        [
            StructPropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            SNew(SComboBox<TSharedPtr<UClass>>)
                .OptionsSource(&ValidClasses)
                .InitiallySelectedItem(SelectedClass)
                .OnGenerateWidget_Lambda([](TSharedPtr<UClass> InClass)
                    {
                        FString Label = TEXT("Invalid");
                        if (InClass.IsValid())
                        {
                            if (const auto* Default =
                                InClass->GetDefaultObject<UInformativePrimaryDataAsset>())
                            {
                                Label = Default->AssetTypeName.ToString();
                            }
                        }
                        return SNew(STextBlock)
                            .Text(FText::FromString(Label))
                            .Font(FAppStyle::Get().GetFontStyle("PropertyWindow.NormalFont"));
                    })
                .OnSelectionChanged_Lambda([this](TSharedPtr<UClass> NewSelection, ESelectInfo::Type)
                    {
                        if (NewSelection.IsValid() &&
                            NewSelection->IsChildOf(UInformativePrimaryDataAsset::StaticClass()))
                        {
                            SelectedClass = NewSelection;
                            ClassHandle->SetValueFromFormattedString(
                                NewSelection->GetPathName());
                        }
                    })
                .Content()
                [
                    SNew(STextBlock)
                        .Text_Lambda([this]() -> FText
                            {
                                if (SelectedClass.IsValid())
                                {
                                    if (const auto* Default =
                                        SelectedClass->GetDefaultObject<UInformativePrimaryDataAsset>())
                                    {
                                        return FText::FromName(
                                            Default->AssetTypeName);
                                    }
                                }
                                return LOCTEXT("NoneSelected", "Select Asset Type");
                            })
                        .Font(FAppStyle::Get().GetFontStyle("PropertyWindow.NormalFont"))
                ]
        ];
}

#undef LOCTEXT_NAMESPACE
