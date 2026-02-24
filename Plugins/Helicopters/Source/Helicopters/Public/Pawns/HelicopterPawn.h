// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/CombinedBundlePawn.h"
#include "Interfaces/Control/PawnControlInterface.h"
#include "HelicopterPawn.generated.h"

UCLASS()
class HELICOPTERS_API AHelicopterPawn : public ACombinedBundlePawn
{
	GENERATED_BODY()

public:
	AHelicopterPawn();

private:
	virtual EPawnControlType GetPawnControlType_Implementation() const override;
};
