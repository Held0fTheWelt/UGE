#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GuidManagerEntry.generated.h"

USTRUCT(BlueprintType)
struct CORE_STRUCTURE_API FGuidManagerEntry : public FTableRowBase
{
	GENERATED_BODY()

public:
	FGuidManagerEntry()
	{
		ObjectPath = FString();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable", meta = (RowType = "ObjectPath"))
	FString ObjectPath;
};
