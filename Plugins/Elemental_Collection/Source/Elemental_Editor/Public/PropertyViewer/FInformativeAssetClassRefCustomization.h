#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"
#include "Structs/InformativeAssetClassRef.h"
#include "UObject/UObjectIterator.h"

/**
 * Customizes how FInformativeAssetClassRef is displayed in the details panel.
 * Provides a dropdown listing all concrete UInformativePrimaryDataAsset subclasses
 * and shows their AssetTypeName directly.
 */
class ELEMENTAL_EDITOR_API FInformativeAssetClassRefCustomization : public IPropertyTypeCustomization
{
public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    // IPropertyTypeCustomization interface
    virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
        FDetailWidgetRow& HeaderRow,
        IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

    virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
        IDetailChildrenBuilder& StructBuilder,
        IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
    {
        // No children
    }

private:
    // Stored between lambdas to ensure valid lifetime
    TSharedPtr<IPropertyHandle> ClassHandle;
    TArray<TSharedPtr<UClass>> ValidClasses;
    TSharedPtr<UClass> SelectedClass;
};