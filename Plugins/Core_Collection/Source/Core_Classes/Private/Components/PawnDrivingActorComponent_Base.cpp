// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PawnDrivingActorComponent_Base.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"

#include "Interfaces/PossessedByControllerInterface.h"
#include "Interfaces/RegisterComponentInterface.h"

#include "Libs/CoreInformationFunctionLibrary.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"
#include "Logging/Core_Classes_Logging.h"

#include "Structs/InformationSet.h"

UPawnDrivingActorComponent_Base::UPawnDrivingActorComponent_Base(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// Initialize other properties if necessary
	PawnOwner = nullptr; // This will be set when the component is attached to a pawn
	PawnController = nullptr; // This will be set when the component is attached to a pawn

	bDoNotRegisterOnPossessControl = false; // Default value, can be set in derived classes or at runtime
}

void UPawnDrivingActorComponent_Base::BeginPlay()
{
	Super::BeginPlay();

	PawnOwner = Cast<APawn>(GetOwner());

	if (!PawnOwner)
	{
		return;
	}


	PawnController = PawnOwner->GetController();

	if(!PawnController)
	{
		UE_LOGFMT(Log_PawnDrivingComponent, Verbose, "{Time}: {Line} {Class}: PawnController is null for PawnOwner: {PawnOwner}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("PawnOwner", *PawnOwner->GetName()));
		//return;
	}

	if (bDoNotRegisterOnPossessControl)
	{
		UE_LOGFMT(Log_PawnDrivingComponent, VeryVerbose, "{Time}: {Line} {Class}: bDoNotRegisterOnPossessControl is true, skipping registration",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (PawnOwner->GetClass()->ImplementsInterface(UPossessedByControllerInterface::StaticClass()))
	{
		IPossessedByControllerInterface* Interface = Cast<IPossessedByControllerInterface>(PawnOwner);

		if (!Interface)
		{
			UE_LOGFMT(Log_PawnDrivingComponent, Warning, "{Time}: {Line} {Class}: PawnOwner does not implement IPossessedByControllerInterface",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
		}

		FOnPawnPossessedByController& PossessEvent = Interface->GetOnPawnPossessedByController();
		// Delegate binden (am besten mit einer UFUNCTION in deinem Listener)
		PossessEvent.AddDynamic(this, &UPawnDrivingActorComponent_Base::OnPossessFired);

		UE_LOGFMT(Log_PawnDrivingComponent, Verbose, "{Time}: {Line} {Class}: PawnOwner implements IPossessedByControllerInterface, binding OnPossessFired: {Owner}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", GetFName()));
	}
	else
	{
		UE_LOGFMT(Log_PawnDrivingComponent, Verbose, "{Time}: {Line} {Class}: PawnOwner does not implement IPossessedByControllerInterface {Component} {Owner}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Component", GetFName()),
			("Owner", GetOwner()->GetFName()));
	}

	OnPossessFired(PawnController);
}

bool UPawnDrivingActorComponent_Base::IsPawnOwnerValid() const
{
	return IsValid(PawnOwner);
}

bool UPawnDrivingActorComponent_Base::IsPawnControllerValid() const
{
	return IsValid(PawnController);
}

#include "TimerManager.h"

void UPawnDrivingActorComponent_Base::OnPossessFired(AController* InController)
{
	FTimerHandle TimerHandle_NextTick;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_NextTick, this, &UPawnDrivingActorComponent_Base::OnNextTick, 0.01f, false);
}

void UPawnDrivingActorComponent_Base::OnNextTick()
{
	PawnController = PawnOwner ? PawnOwner->GetController() : nullptr;

	if (!PawnController)
	{
		UE_LOGFMT(Log_PawnDrivingComponent, Verbose, "{Time}: {Line} {Class}: OnPossess fired with null Controller",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	UE_LOGFMT(Log_PawnDrivingComponent, Verbose, "{Time}: {Line} {Class}: OnPossess fired with Controller: {Controller} on Class {Class} - {Name}",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Controller", *PawnController->GetName()),
		("Class", *GetClass()->GetName()),
		("Name", GetFName()));

	ProcessPossess();
}


void UPawnDrivingActorComponent_Base::ProcessPossess()
{
	if (PawnController->GetClass()->ImplementsInterface(URegisterComponentInterface::StaticClass()))
	{
		IRegisterComponentInterface::Execute_RegisterComponentWithInterface(PawnController, this);
		UE_LOGFMT(Log_PawnDrivingComponent, Verbose, "{Time}: {Line} {Class}: Registered component with PawnController",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
	else
	{
		UE_LOGFMT(Log_PawnDrivingComponent, Verbose, "{Time}: {Line} {Class}: PawnController does not implement IRegisterComponentInterface",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
}

TArray<FInformationSet> UPawnDrivingActorComponent_Base::GetSettingsVariableSets_Implementation() const
{
	TArray<FInformationSet> InformationSets = TArray<FInformationSet>();

	FInformationSet PawnDrivingInfoSet = UCoreInformationFunctionLibrary::GetInformationSet(
		"Pawn Driven Information",
		"",
		""
	);

	UCoreInformationFunctionLibrary::AddInformationValue(
		PawnDrivingInfoSet,
		"Do not register on Possess Control",
		bDoNotRegisterOnPossessControl ? "True" : "False",
		EValueType::VT_BOOL
	);

	if(PawnOwner)
	{
		UCoreInformationFunctionLibrary::AddInformationValue(
			PawnDrivingInfoSet,
			"Pawn Owner",
			PawnOwner->GetName(),
			EValueType::VT_STRING
		);
	}
	else
	{
		UCoreInformationFunctionLibrary::AddInformationValue(
			PawnDrivingInfoSet,
			"Pawn Owner",
			"Null",
			EValueType::VT_STRING
		);
	}

	if(PawnController)
	{
		UCoreInformationFunctionLibrary::AddInformationValue(
			PawnDrivingInfoSet,
			"Pawn Controller",
			PawnController->GetName(),
			EValueType::VT_STRING
		);
	}
	else
	{
		UCoreInformationFunctionLibrary::AddInformationValue(
			PawnDrivingInfoSet,
			"Pawn Controller",
			"Null",
			EValueType::VT_STRING
		);
	}

	InformationSets.Add(PawnDrivingInfoSet);

	InformationSets.Append(Super::GetSettingsVariableSets_Implementation());

	return InformationSets;
}
