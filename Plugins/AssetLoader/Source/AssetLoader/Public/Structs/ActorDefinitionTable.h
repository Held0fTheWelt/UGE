#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ActorDefinitionTable.generated.h"

/**
 * Shared base for actor and component definition rows.
 */
USTRUCT(BlueprintType)
struct FActorDefinitionTable : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName ActorDefinitionName;
};