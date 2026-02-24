// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawns/VehiclePawn.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "Helper/EngineAssignmentDataHelper.h"

DEFINE_LOG_CATEGORY(Log_VehiclePawn);

AVehiclePawn::AVehiclePawn()
{
	// Configure the car mesh
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Vehicle"));
}

void AVehiclePawn::BeginPlay()
{	
	Super::BeginPlay();

}

//void AVehiclePawn::AssetLoaded_Implementation(UObject* Object, int32 Index)
//{
//	if (Object == nullptr)
//	{
//		UE_LOGFMT(Log_VehiclePawn, Error, "{Time}: {Line} {Class}: Loaded object is null",
//			("Class", GET_CLASSNAME_WITH_FUNCTION),
//			("Line", GET_LINE_NUMBER),
//			("Time", GET_CURRENT_TIME));
//		return;
//	}
//

//
//	Super::AssetLoaded_Implementation(Object, Index);
//}

