// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/InteractManageGameInstanceSubsystem.h"

#include "DataAssets/ActorCollectionDataAsset.h"

#include "Engine/World.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"

#include "Interfaces/InteractDummyInterface.h"
#include "Interfaces/OccupiableInterface.h"
#include "Interfaces/SpawnPointInformationInterface.h"

#include "Logging/Core_Classes_Logging.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "Pawns/CombinedWheeledVehiclePawn.h"

#include "Subsystems/SpawnInstanceSubsystem.h"

void UInteractManageGameInstanceSubsystem::InteractedWithActor(AActor* InteractedActor, AActor* Interactor, AController* InteractorInstigator)
{
	UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: InteractedWithActor: InteractedActor={InteractedActor} Interactor={Interactor} Controller={Controller}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("InteractedActor", InteractedActor ? InteractedActor->GetFName() : NAME_None),
		("Interactor", Interactor ? Interactor->GetFName() : NAME_None),
		("Controller", InteractorInstigator ? InteractorInstigator->GetFName() : NAME_None));

	if (!CheckInputValues(InteractedActor, Interactor, InteractorInstigator))
	{
		return;
	}

	if (InteractedActor == Interactor)
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: [EXIT] InteractedActor==Interactor -> AbandonOldPawn (exit vehicle).",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		AbandonOldPawn(InteractedActor, InteractorInstigator);
	}
	else
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: [ENTER] InteractedActor!=Interactor -> OccupyGivenPawn.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		OccupyGivenPawn(InteractedActor, InteractorInstigator);
	}
}

void UInteractManageGameInstanceSubsystem::AbandonOldPawn(AActor* InteractedActor, AController* InteractorInstigator)
{
	if (InteractedActor->GetClass()->ImplementsInterface(UOccupiableInterface::StaticClass()))
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: [EXIT] AbandonOldPawn: vehicle {Owner} implements Occupiable -> calling SpawnNewDefaultPawn for {Controller}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", InteractedActor->GetFName().ToString()),
			("Controller", *InteractorInstigator->GetName()));
		SpawnNewDefaultPawn(InteractorInstigator);
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: [EXIT] SpawnNewDefaultPawn returned (exit flow done).",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
	else
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: [EXIT BLOCKED] Actor {Owner} does not implement IOccupiableInterface – cannot abandon/exit.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", InteractedActor->GetFName().ToString()));
	}
}

void UInteractManageGameInstanceSubsystem::SpawnNewDefaultPawn(AController* InteractorInstigator)
{
	if (!InteractorInstigator)
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: InteractorInstigator is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	// Ensure the instigator is a player controller
	UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, VeryVerbose, "{Time}: {Line} {Class}: Cast {Controller} to PlayerController.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Controller", *InteractorInstigator->GetName()));

	APlayerController* Controller = Cast<APlayerController>(InteractorInstigator);

	UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Verbose, "{Time}: {Line} {Class}: Casted {Controller} to PlayerController.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Controller", *InteractorInstigator->GetName()));
	if(!Controller)
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: InteractorInstigator is not a PlayerController.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	// Get the pawn from the controller
	UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, VeryVerbose, "{Time}: {Line} {Class}: Getting Pawn from {Controller}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Controller", *InteractorInstigator->GetName()));
	APawn* Pawn = Controller->GetPawn();

	if(!Pawn)
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: InteractorInstigator has no Pawn to abandon.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	// Get the spawn transform from the pawn
	UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, VeryVerbose, "{Time}: {Line} {Class}: Getting Transform from Pawn {Pawn}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Pawn", *Pawn->GetName()));
	FTransform SpawnTransform = Pawn->GetTransform();

	// Check if the pawn implements the SpawnPointInformationInterface
	UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, VeryVerbose, "{Time}: {Line} {Class}: Checking if Pawn {Pawn} implements SpawnPointInformationInterface.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("Pawn", *Pawn->GetName()));
	if (Pawn->GetClass()->ImplementsInterface(USpawnPointInformationInterface::StaticClass()))
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Verbose, "{Time}: {Line} {Class}: Pawn {Pawn} implements SpawnPointInformationInterface, getting spawn point for controller {Controller}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Pawn", *Pawn->GetName()),
			("Controller", *InteractorInstigator->GetName()));
		SpawnTransform = ISpawnPointInformationInterface::Execute_GetSpawnPointForController(Pawn, InteractorInstigator);
	}
	else
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: Pawn {Pawn} does not implement SpawnPointInformationInterface, using default transform.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Pawn", *Pawn->GetName()));
	}

	// Check if the game instance subsystem is valid
	if (USpawnInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USpawnInstanceSubsystem>())
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: [EXIT] SpawnNewDefaultPawn: calling SpawnInstanceSubsystem->SpawnDefaultPawn for {Controller}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Controller", *InteractorInstigator->GetName()));
		Subsystem->SpawnDefaultPawn(Controller, SpawnTransform);
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: [EXIT] SpawnDefaultPawn completed for {Controller}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Controller", *InteractorInstigator->GetName()));
	}
	else
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: [EXIT BLOCKED] SpawnInstanceSubsystem is null – cannot spawn default pawn for exit.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
}

void UInteractManageGameInstanceSubsystem::OccupyGivenPawn(AActor* InteractedActor, AController* InteractorInstigator)
{
	if (InteractedActor->GetClass()->ImplementsInterface(UOccupiableInterface::StaticClass()))
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, VeryVerbose, "{Time}: {Line} {Class}: Occupying pawn {Owner} by {Controller}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", InteractedActor->GetFName().ToString()),
			("Controller", *InteractorInstigator->GetName()));
		InteractorInstigator->UnPossess();
		InteractorInstigator->Possess(Cast<APawn>(InteractedActor));
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Verbose, "{Time}: {Line} {Class}: Pawn {Owner} occupied by {Controller}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", InteractedActor->GetFName().ToString()),
			("Controller", *InteractorInstigator->GetName()));
	}
	else
	{
		if (InteractedActor->GetClass()->ImplementsInterface(UInteractDummyInterface::StaticClass()))
		{
			UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Verbose, "{Time}: {Line} {Class}: Actor {Owner} implements IInteractDummyInterface, changing entities.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Owner", InteractedActor->GetFName().ToString()));
			LookupDefinedCollectionDummies(InteractedActor, InteractorInstigator);
		}
		else
		{
			UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: Actor {Owner} does not implement IOccupiableInterface, cannot occupy.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Owner", InteractedActor->GetFName().ToString()));
		}
	}
}

void UInteractManageGameInstanceSubsystem::LookupDefinedCollectionDummies(AActor* InteractedActor, AController* InteractorInstigator)
{
	if (ActorCollectionDataAssets.Num() == 0)
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: No ActorCollectionDataAssets defined.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
	else
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Verbose, "{Time}: {Line} {Class}: Looking up defined collection dummies for {Owner} in {num} items.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", InteractedActor->GetFName().ToString()),
			("num", ActorCollectionDataAssets.Num()));
	}
	
	for (const TSoftObjectPtr<UActorCollectionDataAsset>& PrimaryDataAsset : ActorCollectionDataAssets)
	{
		if (!PrimaryDataAsset.IsValid())
		{
			UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: PrimaryDataAsset is not valid.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
			continue;
		}

		if (InteractedActor->IsA(PrimaryDataAsset->DummyActor.Get()))
		{
			UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Verbose, "{Time}: {Line} {Class}: Found matching dummy actor {DummyActor} for {Owner}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("DummyActor", *PrimaryDataAsset->DummyActor->GetName()),
				("Owner", InteractedActor->GetFName().ToString()));

			if (!PrimaryDataAsset->DefaultActor.IsValid())
			{
				UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: DefaultActor is not valid in PrimaryDataAsset {PrimaryDataAsset}.",
					("Class", GET_CLASSNAME_WITH_FUNCTION),
					("Line", GET_LINE_NUMBER),
					("Time", GET_CURRENT_TIME),
					("PrimaryDataAsset", *PrimaryDataAsset.GetAssetName()));
				return;
			}

			SpawnNewDefinedPawn(InteractedActor, PrimaryDataAsset->DefaultActor.Get(), InteractorInstigator);
		}
		else
		{
			UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Verbose, "{Time}: {Line} {Class}: No matching dummy actor found for {Owner} in PrimaryDataAsset {PrimaryDataAsset}.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Owner", InteractedActor->GetFName().ToString()),
				("PrimaryDataAsset", *PrimaryDataAsset.GetAssetName()));
		}
	}
}

void UInteractManageGameInstanceSubsystem::SpawnNewDefinedPawn(AActor* InteractedDummyActor, TSubclassOf<AActor> DefinedActor, AController* InteractorInstigator)
{
	if (!DefinedActor)
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: DefinedActor is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if(!InteractorInstigator)
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: InteractorInstigator is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if(!InteractedDummyActor)
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: InteractedDummyActor is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	FTransform SpawnTransform = InteractedDummyActor->GetTransform();

	UGameInstance* GameInstance = GetGameInstance();

	if(!GameInstance)
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: GameInstance is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	USpawnInstanceSubsystem* SpawnSubsystem = GameInstance->GetSubsystem<USpawnInstanceSubsystem>();

	if(!SpawnSubsystem)
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: SpawnInstanceSubsystem is null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	AActor* SpawnedActor = SpawnSubsystem->SpawnDefaultActorDeferred(DefinedActor, SpawnTransform);

	if (!SpawnedActor)
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Error, "{Time}: {Line} {Class}: Failed to spawn actor of class {ActorClass}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("ActorClass", *DefinedActor->GetName()));
		return;
	}

	if (SpawnedActor->GetClass()->ImplementsInterface(UAssetDefinitionInformationInterface::StaticClass()))
	{
		IAssetDefinitionInformationInterface::Execute_SetDefinitionAssetName(SpawnedActor, IAssetDefinitionInformationInterface::Execute_GetDefinitionAssetName(InteractedDummyActor));
		UE_LOGFMT(Log_DefinedCollectionSpawner, VeryVerbose, "{Time}: {Line} {Class}: SetDefinitionAssetName {Name} on Actor: {Actor}",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME), ("Name", *IAssetDefinitionInformationInterface::Execute_GetDefinitionAssetName(InteractedDummyActor).ToString()), ("Actor", SpawnedActor->GetFName().ToString()));
	}
	else
	{
		UE_LOGFMT(Log_DefinedCollectionSpawner, Warning, "{Time}: {Line} {Class}: Actor does not implement IAssetDefinitionInformationInterface. Cannot set DefinitionAssetName.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
	}

	SpawnedActor->FinishSpawning(InteractedDummyActor->GetActorTransform());

	InteractedDummyActor->Destroy();

	UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Verbose, "{Time}: {Line} {Class}: Successfully spawned actor of class {ActorClass} for {Controller}.",
		("Class", GET_CLASSNAME_WITH_FUNCTION),
		("Line", GET_LINE_NUMBER),
		("Time", GET_CURRENT_TIME),
		("ActorClass", *DefinedActor->GetName()),
		("Controller", *InteractorInstigator->GetName()));

	OccupyGivenPawn(SpawnedActor, InteractorInstigator);
}

void UInteractManageGameInstanceSubsystem::StoppedInteractWithActor(AActor* InteractedActor, AActor* Interactor, AController* InteractorInstigator)
{
	
	if (!CheckInputValues(InteractedActor, Interactor, InteractorInstigator))
	{
		return;
	}


}

bool UInteractManageGameInstanceSubsystem::CheckInputValues(AActor* InteractedActor, AActor* Interactor, AController* InteractorInstigator) const
{
	if (!InteractedActor || !Interactor || !InteractorInstigator)
	{
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Warning, "{Time}: {Line} {Class}: One or more parameters are null.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return false;
	}
	return true;
}

void UInteractManageGameInstanceSubsystem::AddPrimaryDataAsset(TSoftObjectPtr<UActorCollectionDataAsset> PrimaryDataAsset)
{
	if(!PrimaryDataAsset)
	{
		ActorCollectionDataAssets.Add(PrimaryDataAsset);
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Verbose,
			"{Time}: {Line} {Class}: Added PrimaryDataAsset {PrimaryDataAsset} to ActorCollectionDataAssets.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (!ActorCollectionDataAssets.Contains(PrimaryDataAsset))
	{
		ActorCollectionDataAssets.Add(PrimaryDataAsset);
		UE_LOGFMT(Log_InteractManageGameInstanceSubsystem, Verbose,
			"{Time}: {Line} {Class}: Added PrimaryDataAsset {PrimaryDataAsset} to ActorCollectionDataAssets.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("PrimaryDataAsset", *PrimaryDataAsset.GetAssetName()));

	}

}

void UInteractManageGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ActorCollectionDataAssets.Empty();
}

void UInteractManageGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();

	ActorCollectionDataAssets.Empty();
}
