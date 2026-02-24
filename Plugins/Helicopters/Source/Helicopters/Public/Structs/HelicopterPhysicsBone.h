#pragma once

#include "CoreMinimal.h"
#include "Enums/HelicopterPhysicalComponentType.h"
#include "HelicopterPhysicsBone.generated.h"

/**
 * FHelicopterPhysicsBone
 *
 * Structure representing a physics bone in the helicopter's rig.
 */
USTRUCT(BlueprintType)
struct HELICOPTERS_API FHelicopterPhysicsBone
{
	GENERATED_BODY()

public:
	/** Constructor with parameters */
	FHelicopterPhysicsBone(EHelicopterPhysicalComponentType Type) 
		: ComponentType(Type)
		, ComponentBoneName(NAME_None)
	{
	}

	/** Default constructor */
	FHelicopterPhysicsBone()
		: ComponentType(EHelicopterPhysicalComponentType::HPT_NONE)
		, ComponentBoneName(NAME_None)
	{
	}

	/** Type/category of this bone (see EHelicopterPhysicalComponentType) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Helicopter Physics")
	EHelicopterPhysicalComponentType ComponentType;
	/** Name of the bone in the helicopter's rig */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Helicopter Physics")
	FName ComponentBoneName;
};