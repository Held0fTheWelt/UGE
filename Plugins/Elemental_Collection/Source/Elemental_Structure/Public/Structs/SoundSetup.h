#pragma once

#include "CoreMinimal.h"
#include "Enums/SoundType.h"
#include "SoundSetup.generated.h"

/**
 */
USTRUCT(BlueprintType)
struct FSoundSetup
{
	GENERATED_BODY()

public:
	FSoundSetup()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	EElementalSoundType SoundType = EElementalSoundType::ST_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TSoftObjectPtr<USoundBase> Sound;
};