// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/AircraftAutopilotAIController.h"

void AAircraftAutopilotAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* Controlled = GetPawn();
	if (!Controlled) return;

	FVector Forward = Controlled->GetActorForwardVector();
	Controlled->AddMovementInput(Forward, 1.0f);
}
