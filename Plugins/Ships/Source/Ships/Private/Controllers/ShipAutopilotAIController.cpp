// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/ShipAutopilotAIController.h"

void AShipAutopilotAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* Controlled = GetPawn();
	if (!Controlled) return;

	FVector Forward = Controlled->GetActorForwardVector();
	Controlled->AddMovementInput(Forward, 1.0f);
}
