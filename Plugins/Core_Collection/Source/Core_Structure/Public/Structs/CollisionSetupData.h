#pragma once

#include "CoreMinimal.h"
#include "CollisionSetupData.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct CORE_STRUCTURE_API FCollisionSetupData
{
	GENERATED_BODY()

public:
	FCollisionSetupData()
	{
		CollisionResponse = ECR_MAX;
		CollisionChannels = TArray<TEnumAsByte<ECollisionChannel>>();
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TEnumAsByte<ECollisionResponse> CollisionResponse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannels;
};
