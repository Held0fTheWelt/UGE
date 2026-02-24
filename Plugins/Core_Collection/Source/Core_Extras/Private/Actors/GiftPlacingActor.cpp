// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GiftPlacingActor.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Blueprint/UserWidget.h"

#include "NiagaraSystem.h"


AGiftPlacingActor::AGiftPlacingActor()
{
	PrimaryActorTick.bCanEverTick = false;

	if (!Root)
	{
		Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		SetRootComponent(Root);
	}

	if (!GiftMesh)
	{
		GiftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GiftMesh"));
		GiftMesh->SetupAttachment(Root);
	}

	if (!BoxCollision)
	{
		BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
		BoxCollision->SetupAttachment(Root);
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}

	if (!SpawnLocationMesh)
	{
		SpawnLocationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnLocationMesh"));
		SpawnLocationMesh->SetupAttachment(Root);
		SpawnLocationMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnLocationMesh->SetHiddenInGame(true);
	}

	if (!DecoyPosition_1)
	{
		DecoyPosition_1 = CreateDefaultSubobject<USceneComponent>(TEXT("DecoyPosition_1"));
		DecoyPosition_1->SetupAttachment(Root);
	}

	if (!DecoyPosition_2)
	{
		DecoyPosition_2 = CreateDefaultSubobject<USceneComponent>(TEXT("DecoyPosition_2"));
		DecoyPosition_2->SetupAttachment(Root);
	}

	if (!DecoyPosition_3)
	{
		DecoyPosition_3 = CreateDefaultSubobject<USceneComponent>(TEXT("DecoyPosition_3"));
		DecoyPosition_3->SetupAttachment(Root);
	}

	if (!DecoyPosition_4)
	{
		DecoyPosition_4 = CreateDefaultSubobject<USceneComponent>(TEXT("DecoyPosition_4"));
		DecoyPosition_4->SetupAttachment(Root);
	}

	if (!DecoyPosition_5)
	{
		DecoyPosition_5 = CreateDefaultSubobject<USceneComponent>(TEXT("DecoyPosition_5"));
		DecoyPosition_5->SetupAttachment(Root);
	}

	if (!DecoyPosition_6)
	{
		DecoyPosition_6 = CreateDefaultSubobject<USceneComponent>(TEXT("DecoyPosition_6"));
		DecoyPosition_6->SetupAttachment(Root);
	}

	// Set default values for the gift actor class
	GiftActorClass = nullptr; // Set to a valid class in the editor or via code later
	OpenedSound = nullptr; // Set to a valid sound asset in the editor or via code later
	EffectsSound = nullptr; // Set to a valid sound asset in the editor or via code later
	DecoyEffect = nullptr; // Set to a valid Niagara effect in the editor or via code later
	SolveInformationWidget = nullptr; // Set to a valid widget class in the editor or via code later

}

void AGiftPlacingActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (GiftMesh && GiftMesh->GetStaticMesh())
	{
		// Bounding Box des Meshes holen (lokaler Raum)
		const FBoxSphereBounds Bounds = GiftMesh->Bounds;
		const FVector BoxExtent = Bounds.BoxExtent;

		// Skalierung zurückrechnen, damit Box korrekt relativ zur Meshgröße ist
		FVector RelativeBoxExtent = BoxExtent / GiftMesh->GetComponentScale() * 2;

		RelativeBoxExtent.Z /= 2;

		BoxCollision->SetBoxExtent(RelativeBoxExtent * GiftMesh->GetRelativeScale3D());

		//GiftMesh->SetRelativeLocation(FVector(0.f, 0.f, RelativeBoxExtent.Z ));
	}
}

void AGiftPlacingActor::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AGiftPlacingActor::ActorBeginOverlap);
}

void AGiftPlacingActor::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this && GiftActorClass.IsValid())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* SpawnedGift = GetWorld()->SpawnActor<AActor>(GiftActorClass.Get(), GetTransform(), SpawnParams);
		if (SpawnedGift)
		{
			// Optional: Hier können Sie weitere Logik hinzufügen, z.B. das Geschenk an den Spieler übergeben
			UE_LOG(LogTemp, Log, TEXT("Gift spawned: %s"), *SpawnedGift->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn gift actor."));
		}

		Destroy(); // Zerstört den Placing Actor nach dem Platzieren des Geschenks
	}
}

