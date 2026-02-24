// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CombinedBundleCharacter.h"
#include "HumanoidCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HUMANOIDS_API AHumanoidCharacter : public ACombinedBundleCharacter
{
	GENERATED_BODY()

public:
	AHumanoidCharacter();

protected:
	virtual EPawnControlType GetPawnControlType_Implementation() const override;
};
