#pragma once

#include "CoreMinimal.h"
#include "Enums/ValueType.h"
#include "InformationSet.generated.h"

USTRUCT(BlueprintType)
struct FInformationSetInformation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FString VisibleName = ""; // Value of the information set
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FString Value = ""; // Value of the information set
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	EValueType ValueType = EValueType::VT_NONE; // Type of the value (e.g., Bool, Int, Float, String, Enum, Struct, Class)
};

USTRUCT(BlueprintType)
struct FInformationSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FString InformationCategory = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FString InformationName = ""; // Name of the information set
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FString InformationDescription = ""; // Description of the information set
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	TArray<FInformationSetInformation> InformationValues = TArray<FInformationSetInformation>(); // Values of the information set
};
