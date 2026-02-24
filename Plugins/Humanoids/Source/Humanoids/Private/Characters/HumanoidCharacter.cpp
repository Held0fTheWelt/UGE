// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HumanoidCharacter.h"

#include "Animation/AnimInstance.h"

#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Interfaces/InteractInterface.h"
#include "Interfaces/HumanoidSenseInterface.h"

#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

AHumanoidCharacter::AHumanoidCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	// „QueryCollision“ ist Dein Custom Object-Channel #2
	GetCapsuleComponent()->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_GameTraceChannel2,
		ECR_Overlap
	);

	GetCapsuleComponent()->BodyInstance.UpdatePhysicsFilterData();

	bSetMeshToDefaultSkeletalMeshComponent = false;
}

EPawnControlType AHumanoidCharacter::GetPawnControlType_Implementation() const
{
	return EPawnControlType::PCT_HUMANOID;
}