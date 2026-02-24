// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interfaces/HelicopterAnimInstanceInterface.h"
#include "HelicopterAnimInstance.generated.h"

/**
 * AnimInstance for helicopter skeletal meshes.
 *
 * Provides two usage paths:
 * 1. Legacy delegate-based: Rotor_Main / Rotor_Tail set via IHelicopterAnimInstanceInterface delegates.
 * 2. SKM push-based: SKM_PhysicsBasedHelicopterComponent calls setters each frame,
 *    NativeUpdateAnimation computes derived values for AnimBP "Modify Bone" nodes.
 */
UCLASS()
class HELICOPTERS_API UHelicopterAnimInstance : public UAnimInstance, public IHelicopterAnimInstanceInterface
{
	GENERATED_BODY()

public:
	UHelicopterAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// ── Legacy delegate-based properties (unchanged) ────────────────

	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation")
	float Rotor_Main;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation")
	float Rotor_Tail;

	// ── Input properties (set by SKM each frame via setters) ────────

	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Input")
	float MainRotorSpeed = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Input")
	float TailRotorSpeed = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Input")
	float CollectiveInput = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Input")
	FVector2D CalibratedCyclic = FVector2D::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Input")
	float CalibratedPedals = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Input")
	float CurrentDiscTiltPitch = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Input")
	float CurrentDiscTiltRoll = 0.f;

	// ── Configuration (set once by SKM during setup) ────────────────

	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Config")
	float RotorRotationScalar = -360.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Config")
	float MFInputScalar = 20.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Config")
	float MRInputScalar = -20.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Config")
	float PedalsInputScalar = -20.f;

	// ── Computed outputs (calculated in NativeUpdateAnimation) ───────

	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Computed")
	float MainRotorYaw = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Computed")
	float TailRotorPitch = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Computed")
	float MainBladeCollectiveRoll = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Computed")
	float TailBladePedalRoll = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Computed")
	FRotator ClocheRotation = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Computed")
	float AileronPitchAngle = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Computed")
	float DiscAnglePitch = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Helicopter Animation|Computed")
	float DiscAngleRoll = 0.f;

public:
	// ── Setters (called by SKM_PhysicsBasedHelicopterComponent) ─────

	void SetMainRotorSpeed(float Value);
	void SetTailRotorSpeed(float Value);
	void SetCollectiveInput(float Value);
	void SetCalibratedCyclic(const FVector2D& Value);
	void SetCalibratedPedals(float Value);
	void SetDiscTilt(float Pitch, float Roll);
	void SetAnimationScalars(float RotorScalar, float MF, float MR, float Pedals);

private:
	// ── Legacy delegate implementation ──────────────────────────────

	FNotifyCurrentRotorMain NotifyCurrentRotorMain;
	virtual FNotifyCurrentRotorMain& GetNotifyCurrentRotorMain() override { return NotifyCurrentRotorMain; }

	FNotifyCurrentRotorTail NotifyCurrentRotorTail;
	virtual FNotifyCurrentRotorTail& GetNotifyCurrentRotorTail() override { return NotifyCurrentRotorTail; }

	UFUNCTION()
	void SetRotorMain(float Value);
	UFUNCTION()
	void SetRotorTail(float Value);
};
