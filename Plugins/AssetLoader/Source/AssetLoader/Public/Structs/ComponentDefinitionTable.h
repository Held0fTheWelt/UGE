#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ComponentDefinitionTable.generated.h"

/**
 * Shared base for actor and component definition rows.
 */
USTRUCT(BlueprintType)
struct FComponentDefinitionTable : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
    FName ComponentDefinitionName;
};