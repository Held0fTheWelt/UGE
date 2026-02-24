// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/HelicopterAnimInstance.h"

UHelicopterAnimInstance::UHelicopterAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Rotor_Main = 0.f;
	Rotor_Tail = 0.f;
}

void UHelicopterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Rotor_Main = 0.f;
	Rotor_Tail = 0.f;

	NotifyCurrentRotorMain.AddDynamic(this, &UHelicopterAnimInstance::SetRotorMain);
	NotifyCurrentRotorTail.AddDynamic(this, &UHelicopterAnimInstance::SetRotorTail);
}

void UHelicopterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 1. Rotor accumulation (continuous spinning)
	MainRotorYaw += MainRotorSpeed * RotorRotationScalar * DeltaSeconds;
	MainRotorYaw = FMath::Fmod(MainRotorYaw, 360.f);
	TailRotorPitch += TailRotorSpeed * RotorRotationScalar * DeltaSeconds;
	TailRotorPitch = FMath::Fmod(TailRotorPitch, 360.f);

	// 2. Blade pitch from collective / pedals
	MainBladeCollectiveRoll = FMath::GetMappedRangeValueClamped(
		FVector2D(1.f, 0.f), FVector2D(-5.f, 0.f), CollectiveInput);
	TailBladePedalRoll = FMath::GetMappedRangeValueClamped(
		FVector2D(1.f, 0.f), FVector2D(-5.f, 5.f), CalibratedPedals);

	// 3. Cloche rotation from cyclic + pedals
	ClocheRotation = FRotator(
		CalibratedCyclic.Y * MFInputScalar,
		CalibratedPedals * PedalsInputScalar,
		CalibratedCyclic.X * MRInputScalar);

	// 4. Aileron pitch from forward cyclic
	AileronPitchAngle = CalibratedCyclic.Y * MFInputScalar;

	// 5. Disc tilt visualization
	DiscAnglePitch = -CurrentDiscTiltPitch;
	DiscAngleRoll = CurrentDiscTiltRoll;
}

// ── Setters ─────────────────────────────────────────────────────────

void UHelicopterAnimInstance::SetMainRotorSpeed(float Value)
{
	MainRotorSpeed = Value;
}

void UHelicopterAnimInstance::SetTailRotorSpeed(float Value)
{
	TailRotorSpeed = Value;
}

void UHelicopterAnimInstance::SetCollectiveInput(float Value)
{
	CollectiveInput = Value;
}

void UHelicopterAnimInstance::SetCalibratedCyclic(const FVector2D& Value)
{
	CalibratedCyclic = Value;
}

void UHelicopterAnimInstance::SetCalibratedPedals(float Value)
{
	CalibratedPedals = Value;
}

void UHelicopterAnimInstance::SetDiscTilt(float Pitch, float Roll)
{
	CurrentDiscTiltPitch = Pitch;
	CurrentDiscTiltRoll = Roll;
}

void UHelicopterAnimInstance::SetAnimationScalars(float RotorScalar, float MF, float MR, float Pedals)
{
	RotorRotationScalar = RotorScalar;
	MFInputScalar = MF;
	MRInputScalar = MR;
	PedalsInputScalar = Pedals;
}

// ── Legacy delegate callbacks ───────────────────────────────────────

void UHelicopterAnimInstance::SetRotorMain(float Value)
{
	Rotor_Main = Value;
}

void UHelicopterAnimInstance::SetRotorTail(float Value)
{
	Rotor_Tail = Value;
}
