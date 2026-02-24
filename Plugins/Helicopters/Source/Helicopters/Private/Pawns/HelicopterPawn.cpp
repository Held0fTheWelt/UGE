// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/HelicopterPawn.h"

AHelicopterPawn::AHelicopterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

EPawnControlType AHelicopterPawn::GetPawnControlType_Implementation() const
{
	return EPawnControlType::PCT_HELICOPTER;
}
