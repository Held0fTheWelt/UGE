#pragma once

#include "CoreMinimal.h"
#include "PrimaryAssetSetArray.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPrimaryAssetSetsChanged);
DECLARE_MULTICAST_DELEGATE(FOnPrimaryAssetSetSelected);

/**
 */
USTRUCT(BlueprintType)
struct FPrimaryAssetSetArray
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TArray<TSoftObjectPtr<class UPrimaryDataAssetSet>> CurrentPrimaryDataAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	int32 SetIndex = 0;    // Füge exakt dieses Feld hinzu:

	FOnPrimaryAssetSetsChanged OnPrimaryAssetSetsChanged;
	FOnPrimaryAssetSetSelected OnPrimaryAssetSetSelected;
};