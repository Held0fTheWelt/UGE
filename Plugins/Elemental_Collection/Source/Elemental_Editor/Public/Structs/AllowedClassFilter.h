#pragma once

#include "CoreMinimal.h"
#include "ClassViewerFilter.h"


class FAllowedClassFilter : public IClassViewerFilter
{
public:
    ECustomAssetDefinitionType DefinitionType = ECustomAssetDefinitionType::None;

    virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
    {
        if (!InClass) return false;

        switch (DefinitionType)
        {
        case ECustomAssetDefinitionType::Actor:
            return InClass->IsChildOf(AActor::StaticClass());
        case ECustomAssetDefinitionType::Component:
            return InClass->IsChildOf(UActorComponent::StaticClass());
        default:
            return false;
        }
    }

    virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
    {
        if (DefinitionType == ECustomAssetDefinitionType::Actor)
            return InUnloadedClassData->IsChildOf(AActor::StaticClass());
        else if (DefinitionType == ECustomAssetDefinitionType::Component)
            return InUnloadedClassData->IsChildOf(UActorComponent::StaticClass());

        return false;
    }
};
