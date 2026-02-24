#pragma once

#include "CoreMinimal.h"
#include "Enums/SpawnPointType.h"
#include "Enums/CollisionType.h"
#include "CollisionSetup.generated.h"

USTRUCT(BlueprintType)
struct FCollisionSetup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	ECollisionType CollisionType = ECollisionType::CVT_SPHERE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FTransform CollisionTransform = FTransform();

	/** Nur sichtbar, wenn CollisionType == Box */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimension",
		meta = (EditCondition = "CollisionType == ECollisionType::CVT_BOX", EditConditionHides))
	FVector BoxDimension = FVector();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimension",
		meta = (EditCondition = "CollisionType == ECollisionType::CVT_CAPSULE", EditConditionHides))
	FVector2D CapsuleDimension = FVector2D();

	/** Nur sichtbar, wenn CollisionType == Sphere */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimension",
		meta = (EditCondition = "CollisionType == ECollisionType::CVT_SPHERE", EditConditionHides))
	float SphereDimension = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bGenerateOverlapEvents = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bCanEverAffectNavigation = false;

	// Add more properties as needed for collision setup

};