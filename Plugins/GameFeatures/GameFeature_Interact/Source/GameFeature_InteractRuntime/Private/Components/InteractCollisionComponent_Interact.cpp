#include "Components/InteractCollisionComponent.h"

#include "Blueprint/UserWidget.h"

#include "Components/BoxComponent.h"
#include "Components/Image.h"

#include "Enums/SenseType.h"

#include "Interfaces/ShouldAddGameFeatureInterface.h"
#include "Interfaces/HumanoidSenseInterface.h"
#include "Interfaces/SceneComponentCatchInterface.h"
#include "Interfaces/RegisterComponentInterface.h"
#include "Interfaces/InteractInterface.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

#include "Logging/GF_Interact_Logging.h"
#include "Logging/GlobalLog.h"
#include "Logging/StructuredLog.h"

#include "GameFeature_Interact_Settings.h"


void UInteractCollisionComponent::OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (bTemporaryTickLockForRegister)
	//{
	//	UE_LOGFMT(Log_Interact, VeryVerbose, "{Time}: {Line} {Class}: Temporary tick lock is active, ignoring overlap event",
	//		("Class", GET_CLASSNAME_WITH_FUNCTION),
	//		("Line", GET_LINE_NUMBER),
	//		("Time", GET_CURRENT_TIME));
	//	return;
	//}

	//bTemporaryTickLockForRegister = true;
	//FTimerHandle TimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInteractCollisionComponent::UnlockTemporaryRegisterLock, .01f, false);


	if (OtherActor && OtherActor != GetOwner())
	{
		UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: Overlap detected with {OtherActor}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("OtherActor", *OtherActor->GetName()));

		if (!CheckLimit())
		{
			UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: Overlap with {OtherActor} ignored – interaction limit reached.",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("OtherActor", *OtherActor->GetName()));
			return;
		}

		if (OtherActor == GetOwner())
		{
			UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: Cannot interact with self: {OtherActor}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("OtherActor", *OtherActor->GetName()));
			return;
		}

		AController* InstigatorController = OtherActor->GetInstigatorController();

		if (!InstigatorController)
		{
			UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: InstigatorController is null for {OtherActor}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("OtherActor", *OtherActor->GetName()));
			return;
		}

		if (InstigatorController == GetWorld()->GetFirstPlayerController())
		{
			ShowWidget();
		}

		if (InstigatorController->GetClass()->ImplementsInterface(URegisterComponentInterface::StaticClass()))
		{
			IRegisterComponentInterface::Execute_RegisterComponentWithInterface(InstigatorController, this);
			UE_LOGFMT(Log_Interact, VeryVerbose, "{Time}: {Line} {Class}: Registered component with PawnController",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
		}
		else
		{
			UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: PawnController does not implement IRegisterComponentInterface",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
		}

		CheckSenseTouch(OtherActor, true);
		CheckSenseVisual(OtherActor, true);

		InteractableActors.AddUnique(OtherActor);
		UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: Actor {OtherActor} added to InteractableActors: Count = {Num}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Num", InteractableActors.Num()),
			("OtherActor", *OtherActor->GetName()));

	}
}

void UInteractCollisionComponent::OnBoxComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//if (bTemporaryTickLockForRegister)
	//{
	//	UE_LOGFMT(Log_Interact, VeryVerbose, "{Time}: {Line} {Class}: Temporary tick lock is active, ignoring overlap event",
	//		("Class", GET_CLASSNAME_WITH_FUNCTION),
	//		("Line", GET_LINE_NUMBER),
	//		("Time", GET_CURRENT_TIME));
	//	return;
	//}

	//bTemporaryTickLockForRegister = true;
	//FTimerHandle TimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInteractCollisionComponent::UnlockTemporaryRegisterLock, .01f, false);
	if (OtherActor && OtherActor != GetOwner())
	{
		UE_LOGFMT(Log_Interact, VeryVerbose, "{Time}: {Line} {Class}: End overlap with {OtherActor}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("OtherActor", *OtherActor->GetName()));

		if (OtherActor == GetOwner())
		{
			UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: Cannot interact with self: {OtherActor}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("OtherActor", *OtherActor->GetName()));
			return;
		}

		AController* InstigatorController = OtherActor->GetInstigatorController();

		if (!InstigatorController)
		{
			UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: InstigatorController is null for {OtherActor}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("OtherActor", *OtherActor->GetName()));
			return;
		}

		if (OtherActor == GetOwner())
		{
			UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: Cannot interact with self: {OtherActor}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("OtherActor", *OtherActor->GetName()));
			return;
		}

		if (InstigatorController == GetWorld()->GetFirstPlayerController())
		{
			RemoveWidget();
		}

		if (InstigatorController->GetClass()->ImplementsInterface(URegisterComponentInterface::StaticClass()))
		{
			IRegisterComponentInterface::Execute_UnregisterComponentWithInterface(InstigatorController, this);
			UE_LOGFMT(Log_Interact, VeryVerbose, "{Time}: {Line} {Class}: Registered component with PawnController",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
		}
		else
		{
			UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: PawnController does not implement IRegisterComponentInterface",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME));
		}

		CheckSenseTouch(OtherActor, false);
		CheckSenseVisual(OtherActor, false);

		if (InteractableActors.Contains(OtherActor))
		{
			UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: Actor {OtherActor} removed from InteractableActors: Count = {Num}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Num", InteractableActors.Num()),
				("OtherActor", *OtherActor->GetName()));
			InteractableActors.Remove(OtherActor);
			UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: Actor {OtherActor} removed from InteractableActors: Count = {Num}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Num", InteractableActors.Num()),
				("OtherActor", *OtherActor->GetName()));
		}
		else
		{
			UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: Actor {OtherActor} not found in InteractableActors: Count = {Num}",
				("Class", GET_CLASSNAME_WITH_FUNCTION),
				("Line", GET_LINE_NUMBER),
				("Time", GET_CURRENT_TIME),
				("Num", InteractableActors.Num()),
				("OtherActor", *OtherActor->GetName()));
		}
	}
}

void UInteractCollisionComponent::Interact_Implementation(AController* InstigatorController)
{
	if(!InstigatorController)
	{
		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: InstigatorController is null",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	AActor* OwnerActor = GetOwner();

	if (OwnerActor == nullptr)
	{
		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: OwnerActor is null",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
	else
	{
		UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: ControllerInteract called for {Owner}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", *GetOwner()->GetName()));
	}

	if(!OwnerActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: OwnerActor does not implement UInteractInterface",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (IInteractInterface* InteractInterface = Cast<IInteractInterface>(OwnerActor))
	{
		InteractInterface->GetOnInteractWithPawn().Broadcast(InstigatorController->GetPawn(), InstigatorController);
		UE_LOGFMT(Log_Interact, VeryVerbose, "{Time}: {Line} {Class}: Interact called for {Owner} by {Controller}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", *GetOwner()->GetName()),
			("Controller", *InstigatorController->GetName()));
	}
	else
	{
		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: OwnerActor does not implement IInteractInterface",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
}

void UInteractCollisionComponent::StopInteracting_Implementation(AController* InstigatorController)
{
	if (!InstigatorController)
	{
		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: InstigatorController is null",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	AActor* OwnerActor = GetOwner();

	if (OwnerActor == nullptr)
	{
		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: OwnerActor is null",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
	else
	{
		UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: ControllerInteract called for {Owner}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", *GetOwner()->GetName()));
	}

	if (!OwnerActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: OwnerActor does not implement UInteractInterface",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}

	if (IInteractInterface* InteractInterface = Cast<IInteractInterface>(OwnerActor))
	{
		InteractInterface->GetOnInteractWithPawn().Broadcast(InstigatorController->GetPawn(), InstigatorController);
		UE_LOGFMT(Log_Interact, VeryVerbose, "{Time}: {Line} {Class}: StopInteracting called for {Owner} by {Controller}",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Owner", *GetOwner()->GetName()),
			("Controller", *InstigatorController->GetName()));
	}
	else
	{
		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: OwnerActor does not implement IInteractInterface. Cannot stop interacting.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
	}
}

void UInteractCollisionComponent::CheckSenseTouch(AActor* Actor, bool On)
{
	if (!Actor)
	{
		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: CheckSenseTouch called with null Actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
	if (Actor->GetClass()->ImplementsInterface(UHumanoidSenseInterface::StaticClass()))
	{
		if (On)
		{
			IHumanoidSenseInterface::Execute_RegisterObject(Actor, GetOwner(), ESenseType::ST_TOUCH);
		}
		else
		{
			IHumanoidSenseInterface::Execute_ForgetObject(Actor, GetOwner(), ESenseType::ST_TOUCH);
		}
	}
	else
	{
		UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: Actor {Actor} does not implement UHumanoidSenseInterface",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Actor", *Actor->GetName()));
	}
}

void UInteractCollisionComponent::CheckSenseVisual(AActor* Actor, bool On)
{
	if (!Actor)
	{
		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: CheckSenseVisual called with null Actor.",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME));
		return;
	}
	if (Actor->GetClass()->ImplementsInterface(UHumanoidSenseInterface::StaticClass()))
	{
		if (On)
		{
			IHumanoidSenseInterface::Execute_RegisterObject(Actor, GetOwner(), ESenseType::ST_VISUAL);
		}
		else
		{
			IHumanoidSenseInterface::Execute_ForgetObject(Actor, GetOwner(), ESenseType::ST_VISUAL);
		}
	}
	else
	{
		UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: Actor {Actor} does not implement UHumanoidSenseInterface",
			("Class", GET_CLASSNAME_WITH_FUNCTION),
			("Line", GET_LINE_NUMBER),
			("Time", GET_CURRENT_TIME),
			("Actor", *Actor->GetName()));
	}
}

//bool UInteractCollisionComponent::Interact_Implementation(AActor* Interactor)
//{
//	if (!CheckLimit())
//	{
//		return false;
//	}
//
//	if (Interactor == nullptr)
//	{
//		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: Interactor is null",
//			("Class", GET_CLASSNAME_WITH_FUNCTION),
//			("Line", GET_LINE_NUMBER),
//			("Time", GET_CURRENT_TIME));
//		return false;
//	}
//	if (InteractableActors.Contains(Interactor))
//	{
//		CurrentInteractingActor = Interactor;
//		if (RemoveWidget())
//		{
//			Limit--;
//		}
//		UE_LOGFMT(Log_Interact, VeryVerbose, "{Time}: {Line} {Class}: Interactor {Interactor} is interacting with {Owner}",
//			("Class", GET_CLASSNAME_WITH_FUNCTION),
//			("Line", GET_LINE_NUMBER),
//			("Time", GET_CURRENT_TIME),
//			("Interactor", *Interactor->GetName()),
//			("Owner", *GetOwner()->GetName()));
//		return true;
//	}
//	UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: Interactor {Interactor} cannot interact with {Owner}",
//		("Class", GET_CLASSNAME_WITH_FUNCTION),
//		("Line", GET_LINE_NUMBER),
//		("Time", GET_CURRENT_TIME),
//		("Interactor", *Interactor->GetName()),
//		("Owner", *GetOwner()->GetName()));
//	return false;
//}
//
//bool UInteractCollisionComponent::IsInteracting_Implementation(AActor* Interactor)
//{
//	if (Interactor == nullptr)
//	{
//		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: Interactor is null",
//			("Class", GET_CLASSNAME_WITH_FUNCTION),
//			("Line", GET_LINE_NUMBER),
//			("Time", GET_CURRENT_TIME));
//		return false;
//	}
//	return CurrentInteractingActor == Interactor;
//}
//
//bool UInteractCollisionComponent::StopInteract_Implementation(AActor* Interactor)
//{
//	if (Interactor == nullptr)
//	{
//		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: Interactor is null",
//			("Class", GET_CLASSNAME_WITH_FUNCTION),
//			("Line", GET_LINE_NUMBER),
//			("Time", GET_CURRENT_TIME));
//		return false;
//	}
//
//	if (CurrentInteractingActor == nullptr)
//	{
//		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: CurrentInteractingActor is null. Cannot stop interaction.",
//			("Class", GET_CLASSNAME_WITH_FUNCTION),
//			("Line", GET_LINE_NUMBER),
//			("Time", GET_CURRENT_TIME));
//		return false;
//	}
//
//	if (CurrentInteractingActor != Interactor)
//	{
//		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: Interactor {Interactor} is not the current interacting actor {CurrentInteractingActor}. Cannot stop interaction.",
//			("Class", GET_CLASSNAME_WITH_FUNCTION),
//			("Line", GET_LINE_NUMBER),
//			("Time", GET_CURRENT_TIME),
//			("Interactor", *Interactor->GetName()),
//			("CurrentInteractingActor", *CurrentInteractingActor->GetName()));
//		return false;
//	}
//
//	CurrentInteractingActor = nullptr;
//	return true;
//}
//
//bool UInteractCollisionComponent::CanInteract_Implementation(AActor* Interactor) const
//{
//	if (!CheckLimit())
//	{
//		return false;
//	}
//
//	if (Interactor == nullptr)
//	{
//		UE_LOGFMT(Log_Interact, Warning, "{Time}: {Line} {Class}: Interactor is null",
//			("Class", GET_CLASSNAME_WITH_FUNCTION),
//			("Line", GET_LINE_NUMBER),
//			("Time", GET_CURRENT_TIME));
//		return false;
//	}
//	UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: InteractableActors: Count = {Num}",
//		("Class", GET_CLASSNAME_WITH_FUNCTION),
//		("Line", GET_LINE_NUMBER),
//		("Time", GET_CURRENT_TIME),
//		("Num", InteractableActors.Num()));
//	UE_LOGFMT(Log_Interact, Verbose, "{Time}: {Line} {Class}: InteractableActors contains Actor {Interactor}= {Result}",
//		("Class", GET_CLASSNAME_WITH_FUNCTION),
//		("Line", GET_LINE_NUMBER),
//		("Time", GET_CURRENT_TIME),
//		("Interactor", *Interactor->GetName()),
//		("Result", InteractableActors.Contains(Interactor)));
//	return InteractableActors.Contains(Interactor);
//}