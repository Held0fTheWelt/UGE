#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "StaticMeshCollectionItem.generated.h"

/**
 * FSpawnPointsCollection
 *
 * Container for a group of spawn points of a single type (e.g., player, enemy, camera, etc.).
 */
USTRUCT(BlueprintType)
struct FStaticMeshCollectionItem
{
	GENERATED_BODY()

public:
	FStaticMeshCollectionItem()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	FName ComponentName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Transform)
	FTransform ComponentTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StaticMesh)
	TSoftObjectPtr<class UStaticMesh> ComponentMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	TSoftObjectPtr<class UMaterialInterface> ComponentMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sockets)
	FName ComponentToAttachName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
	bool bSimulatePhysics = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
	float Mass = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
	float LinearDamping = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
	float AngularDamping = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
	FVector CenterOfMassOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
	float PositionSolverIterationCount = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
	float VelocitySolverIterationCount = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Physics)
	float StabilizationThreshold = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	bool bSimulationGeneratesHitEvens = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	TSoftObjectPtr<UPhysicalMaterial> PhysicalMaterialOverride = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	bool bGenerateOverlapEvents = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	FName CollisionProfileName = TEXT("BlockAllDynamic");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled = ECollisionEnabled::QueryAndPhysics;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	TEnumAsByte<ECollisionChannel> ObjectType = ECollisionChannel::ECC_WorldStatic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	FCollisionResponseContainer CollisionResponses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visibility)
	bool bVisible = true;
};