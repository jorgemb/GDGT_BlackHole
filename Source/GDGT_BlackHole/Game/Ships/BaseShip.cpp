// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#include "BaseShip.h"
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>
#include <GameFramework/PlayerController.h>
#include <ConstructorHelpers.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Materials/MaterialInstanceDynamic.h>
#include "Game/Universe/Planet.h"

// Sets default values
ABaseShip::ABaseShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create basic mesh
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ShipMesh"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh_StaticMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	ShipMesh->SetStaticMesh(ShipMesh_StaticMesh.Object);
	ShipMesh->SetCollisionProfileName("OverlapAllDynamic");
	ShipMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseShip::OnShipBeginOverlap);

	// Add speed variation
	Speed += FMath::RandRange(-SpeedVariationOnSpawn, SpeedVariationOnSpawn);
	SpeedVariationOnTime = FMath::RandRange(0.0f, SpeedVariationOnTime);

	// Find material
	ConstructorHelpers::FObjectFinder<UMaterialInterface> Ship_Material(TEXT("Material'/Game/Materials/Ships/SimpleShipMaterial.SimpleShipMaterial'"));
	ShipMaterial = Ship_Material.Object;
	ShipMesh->SetMaterial(0, ShipMaterial);

	// Set colors
	ColorOwn = FColor::FColor(0, 0, 255, 200);
	ColorEnemy = FColor::FColor(255, 0, 0, 200);
}

void ABaseShip::InitializeShip(AActor* OwnerActor, APlanet * Source, APlanet * Target)
{
	SourcePlanet = Source;
	TargetPlanet = Target;
	ShipOwner = OwnerActor;
}

// Called when the game starts or when spawned
void ABaseShip::BeginPlay()
{
	Super::BeginPlay();
	
	// Set life span
	SetLifeSpan(120.0f);

	// Create material for the ship
	AActor* PlayerControllerAsActor = Cast<AActor>(GetWorld()->GetFirstPlayerController());

	MIShipMaterial = UMaterialInstanceDynamic::Create(ShipMaterial, this);
	if(ShipOwner == PlayerControllerAsActor)
		MIShipMaterial->SetVectorParameterValue(FName("ShipColor"), ColorOwn);
	else
		MIShipMaterial->SetVectorParameterValue(FName("ShipColor"), ColorEnemy);

	ShipMesh->SetMaterial(0, MIShipMaterial);
}

void ABaseShip::OnShipBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (TargetPlanet == OtherActor) {
		APlanet* OtherActorPlanet = Cast<APlanet>(OtherActor);
		if (OtherActorPlanet)
		{
			OtherActorPlanet->ShipLanding(ShipOwner);
		}
		else
		{
			// DEBUG: Add debug message
		}
		Destroy();
	}
}

// Called every frame
void ABaseShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (TargetPlanet) {
		// Calculate new position based on the current position of the target planet
		FVector TargetPlanetDirection = TargetPlanet->GetActorLocation() - GetActorLocation();
		TargetPlanetDirection.Normalize();

		AddActorWorldOffset(TargetPlanetDirection * Speed * DeltaTime);

		// Accelerate ship after some seconds
		const float AccelerationDeltaPerSecond = 100;
		if (GetWorld()->GetTimeSeconds() - CreationTime > SecondsToStartAcceleration) {
			Speed += (AccelerationDeltaPerSecond + SpeedVariationOnTime) * DeltaTime;
		}
	}
}

