#pragma once

#include "CoreMinimal.h"
#include "Enums/DiplomaticStatus.h"
#include "Faction.generated.h"

USTRUCT(BlueprintType)
struct CORE_STRUCTURE_API FFaction
{
	GENERATED_BODY()

public:
	FFaction()
	{
		Name = EName::None;
		UIColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
		DiplomaticStatus = EDiplomaticStatus::DS_NONE;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	FLinearColor UIColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	EDiplomaticStatus DiplomaticStatus;
};
