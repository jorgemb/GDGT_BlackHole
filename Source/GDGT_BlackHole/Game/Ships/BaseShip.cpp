// Copyright (c) 2019 Jorge Luis Martínez Bonilla

#include "BaseShip.h"
#include <Components/StaticMeshComponent.h>
#include <ConstructorHelpers.h>
#include <Kismet/KismetSystemLibrary.h>
#include "Game/Universe/Planet.h"

// Sets default values
ABaseShip::ABaseShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create basic mesh
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ShipMesh"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh_StaticMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	ShipMesh->SetStaticMesh(ShipMesh_StaticMesh.Object);
	ShipMesh->SetCollisionProfileName("OverlapAllDynamic");
	ShipMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseShip::OnShipBeginOverlap);
}

void ABaseShip::InitializeShip(AActor* ShipOwner, APlanet * SourcePlanet, APlanet * TargetPlanet)
{
	this->SourcePlanet = SourcePlanet;
	this->TargetPlanet = TargetPlanet;
	this->ShipOwner = ShipOwner;
}

// Called when the game starts or when spawned
void ABaseShip::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseShip::OnShipBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Collision overlap"));
	if (TargetPlanet == OtherActor) {
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Is target planet"));
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
		const float AccelerationTime = 10;
		const float AccelerationDeltaPerSecond = 100;
		if (GetWorld()->GetTimeSeconds() - CreationTime > AccelerationTime) {
			Speed += AccelerationDeltaPerSecond * DeltaTime;
		}
	}
}

