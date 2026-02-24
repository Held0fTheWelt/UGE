#pragma once

#include "CoreMinimal.h"
#include "SimpleStringMappingTableItem.generated.h"

/**
 * FSimpleStringMappingTableItem
 *
 * A simple structure for mapping one string to another.
 * Used in data tables for quick lookups or transformations.
 */
USTRUCT(BlueprintType)
struct FSimpleStringMappingTableItem
{
	GENERATED_BODY()

public:
	// Default constructor
	FSimpleStringMappingTableItem()
		: From(TEXT("")), To(TEXT(""))
	{
	}

	FSimpleStringMappingTableItem(const FString& InFrom, const FString& InTo)
		: From(InFrom), To(InTo)
	{
	}

	/** The original string to map from */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FString From;

	/** The target string to map to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FString To;
};