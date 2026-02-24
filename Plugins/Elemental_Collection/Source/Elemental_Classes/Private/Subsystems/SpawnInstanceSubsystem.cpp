/*
 * Copyright (c) 2024 Yves Tanas
 * All Rights Reserved.
 *
 * This file is part of the Collections project.
 *
 * Unauthorized copying of this file, via any medium, is strictly prohibited.
 * Proprietary and confidential.
 *
 * This software may be used only as expressly authorized by the copyright holder.
 * Unless required by applicable law or agreed to in writing, software distributed
 * under this license is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 *
 * For licensing inquiries, please contact: yves.tanas@example.com
 *
 * Contributors:
 *   Yves Tanas <yves.tanas@example.com>
 *
 * -------------------------------------------------------------------------------
 * File:        [SpawnInstanceSubsystem.cpp]
 * Created:     [2025-06-12]
 * Description: [Short file/module purpose]
 * -------------------------------------------------------------------------------
 */
#include "Subsystems/SpawnInstanceSubsystem.h"

#include "Engine/World.h"

#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(Log_SpawnInstanceSubsystem);

AActor* USpawnInstanceSubsystem::SpawnDefaultActor(TSubclassOf<AActor> ActorToSpawn, const FTransform& InTransform)
{
    UWorld* World = GetWorld();
    if (!World)
    {
		UE_LOGFMT(Log_SpawnInstanceSubsystem, Error, "{Time}: {Line} {Class}: World is null. Cannot spawn actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
        return nullptr;
    }

    if (!*ActorToSpawn)  // prüft, ob die Klasse gültig ist
    {
		UE_LOGFMT(Log_SpawnInstanceSubsystem, Error, "{Time}: {Line} {Class}: ActorToSpawn is null or invalid. Cannot spawn actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = nullptr;
    SpawnParams.Instigator = nullptr;
    // SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // optional

    // SpawnActor erwartet direkt den UClass*, also *ActorToSpawn
    AActor* Spawned = World->SpawnActor<AActor>(
        ActorToSpawn,       // TSubclassOf<AActor> konvertiert automatisch zu UClass*
        InTransform.GetLocation(),
        InTransform.GetRotation().Rotator(),
        SpawnParams
    );

	Spawned->SetActorScale3D(InTransform.GetScale3D());

    if (!Spawned)
    {
		UE_LOGFMT(Log_SpawnInstanceSubsystem, Warning, "{Time}: {Line} {Class}: Failed to spawn actor of class {ActorClass}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("ActorClass", *ActorToSpawn->GetName()));
    }
    else
    {
		UE_LOGFMT(Log_SpawnInstanceSubsystem, Verbose, "{Time}: {Line} {Class}: Successfully spawned actor of class {ActorClass} at {Location}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("ActorClass", *ActorToSpawn->GetName()),
			("Location", InTransform.GetLocation().ToString()));
    }

	return Spawned;
}

AActor* USpawnInstanceSubsystem::SpawnDefaultActorDeferred(TSubclassOf<class AActor> ActorToSpawn, const FTransform& InTransform)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOGFMT(Log_SpawnInstanceSubsystem, Error, "{Time}: {Line} {Class}: World is null. Cannot spawn actor.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
        return nullptr;
    }

    if (!*ActorToSpawn)  // prüft, ob die Klasse gültig ist
    {
        UE_LOGFMT(Log_SpawnInstanceSubsystem, Error, "{Time}: {Line} {Class}: ActorToSpawn is null or invalid. Cannot spawn actor.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
        return nullptr;
    }

	FTransform SpawnTransform = InTransform;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = nullptr;
    SpawnParams.Instigator = nullptr;
    // SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // optional

    // SpawnActor erwartet direkt den UClass*, also *ActorToSpawn
    AActor* Spawned = World->SpawnActorDeferred<AActor>(
        ActorToSpawn,       // TSubclassOf<AActor> konvertiert automatisch zu UClass*
        InTransform,
        nullptr, // Owner
        nullptr, // Instigator
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn
    );

    Spawned->SetActorScale3D(InTransform.GetScale3D());

    if (!Spawned)
    {
        UE_LOGFMT(Log_SpawnInstanceSubsystem, Warning, "{Time}: {Line} {Class}: Failed to spawn actor of class {ActorClass}.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("ActorClass", *ActorToSpawn->GetName()));
    }
    else
    {
        UE_LOGFMT(Log_SpawnInstanceSubsystem, Verbose, "{Time}: {Line} {Class}: Successfully spawned actor of class {ActorClass} at {Location}.",
            ("Class", GET_CLASSNAME_WITH_FUNCTION),
            ("Line", GET_LINE_NUMBER),
            ("Time", GET_CURRENT_TIME),
            ("ActorClass", *ActorToSpawn->GetName()),
            ("Location", InTransform.GetLocation().ToString()));
    }

    return Spawned;
}

void USpawnInstanceSubsystem::SpawnDefaultPawn(class APlayerController* PlayerController, const FTransform& InTransform)
{
	if (PlayerController == nullptr)
	{
		UE_LOGFMT(Log_SpawnInstanceSubsystem, Error, "{Time}: {Line} {Class}: PlayerController is null. Cannot spawn default pawn.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

    UWorld* World = GetWorld();
    if (!World)
	{
		UE_LOGFMT(Log_SpawnInstanceSubsystem, Error, "{Time}: {Line} {Class}: World is null. Cannot spawn default pawn.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
	}

    // Hole das aktuelle GameMode (funktioniert nur im Authority/Server-Kontext!)
    AGameModeBase* GameMode = World->GetAuthGameMode();
    if (!GameMode)
    {
		UE_LOGFMT(Log_SpawnInstanceSubsystem, Error, "{Time}: {Line} {Class}: GameMode is null. Cannot spawn default pawn.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
    }
    // Hole die DefaultPawnClass
    TSubclassOf<APawn> PawnClass = GameMode->DefaultPawnClass;
    if (!PawnClass)
    {
		UE_LOGFMT(Log_SpawnInstanceSubsystem, Error, "{Time}: {Line} {Class}: DefaultPawnClass is null in GameMode. Cannot spawn default pawn.",
			("Class", GET_CLASSNAME_WITH_FUNCTION), ("Line", GET_LINE_NUMBER), ("Time", GET_CURRENT_TIME));
		return;
    }

    // Spawn-Params setzen
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Pawn erzeugen
    APawn* NewPawn = World->SpawnActor<APawn>(PawnClass, InTransform.GetLocation(), InTransform.GetRotation().Rotator(), SpawnParams);

    if (NewPawn)
    {
        PlayerController->Possess(NewPawn);
		FRotator NewPawnRotation = InTransform.GetRotation().Rotator();
        NewPawnRotation.Pitch = 0.f;
		NewPawnRotation.Roll = 0.f;
		NewPawn->SetActorRotation(NewPawnRotation);
		PlayerController->SetControlRotation(NewPawnRotation);

		UE_LOGFMT(Log_SpawnInstanceSubsystem, Verbose, "{Time}: {Line} {Class}: Successfully spawned default pawn of class {PawnClass} for PlayerController {PlayerController}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("PawnClass", *PawnClass->GetName()),
			("PlayerController", *PlayerController->GetName()));
    }
    else
    {
		UE_LOGFMT(Log_SpawnInstanceSubsystem, Error, "{Time}: {Line} {Class}: Failed to spawn default pawn of class {PawnClass}.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("PawnClass", *PawnClass->GetName()));
    }
}
