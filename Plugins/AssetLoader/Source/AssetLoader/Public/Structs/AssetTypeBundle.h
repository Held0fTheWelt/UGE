#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AssetTypeBundle.generated.h"

USTRUCT(BlueprintType)
struct FAssetTypeBundle : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
    TArray<FPrimaryAssetType> AssetTypes;
};