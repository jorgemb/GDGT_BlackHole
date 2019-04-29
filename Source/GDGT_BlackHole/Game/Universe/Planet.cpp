// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#include "Planet.h"
#include <Components/StaticMeshComponent.h>
#include <Components/TextRenderComponent.h>
#include <Engine/StaticMesh.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/PlayerController.h>
#include <Materials/MaterialInterface.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <ConstructorHelpers.h>
#include "Game/Ships/BaseShip.h"

APlanet::APlanet()
{
	// Create planet mesh
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ObjectMesh_Mesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> ObjectMesh_Material(TEXT("Material'/Game/Materials/Planets/BasePlanetMaterial.BasePlanetMaterial'"));
	ObjectMesh->SetStaticMesh(ObjectMesh_Mesh.Object);
	ObjectMesh->SetMaterial(0, ObjectMesh_Material.Object);

	// Set selection colors
	ColorSelected = ColorHoverOwn = ColorUnselectedOwn = FColor(0, 0, 128, 255);
	ColorHoverOwn.A = 0.8f;
	ColorUnselectedOwn.A = 0.5f;

	ColorHoverEnemy = ColorUnselectedEnemy = FColor(128, 0, 0, 205);
	ColorUnselectedEnemy.A = 0.5f;

	ColorHoverNeutral = ColorUnselectedNeutral = FColor(128, 128, 128, 205);
	ColorUnselectedNeutral.A = 0.5f;
	
	OrbitColor = ColorHoverNeutral.ToFColor(true);

	// Create selection mesh
	SelectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("SelectionMesh"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> Selection_Mesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> Selection_Material(TEXT("Material'/Game/Materials/SelectionMaterial.SelectionMaterial'"));
	SelectionMaterial = Selection_Material.Object;
	SelectionMesh->bUseDefaultCollision = false;
	SelectionMesh->SetStaticMesh(Selection_Mesh.Object);
	SelectionMesh->SetMaterial(0, Selection_Material.Object);
	SelectionMesh->SetupAttachment(ObjectMesh);
	SelectionMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SelectionMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	SelectionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create spring arm
	NumberHolder = CreateDefaultSubobject<USpringArmComponent>(FName("NumberHolder"));
	NumberHolder->TargetArmLength = 1000;
	NumberHolder->bInheritYaw = false;
	NumberHolder->bInheritPitch = false;
	NumberHolder->bInheritRoll = false;
	NumberHolder->SetupAttachment(Root);
	NumberHolder->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	// Set planet scale
	SetActorScale3D(FVector(4.0f, 4.0f, 4.0f));

	// Add spring arm and units counter
	UnitsText = CreateDefaultSubobject<UTextRenderComponent>(FName("UnitsText"));
	UnitsText->SetTextRenderColor(FColor::Cyan);
	UnitsText->SetRelativeRotation(FRotator(-20.0f, 180.0f, 0.0f));
	UnitsText->SetXScale(4.0f);
	UnitsText->SetYScale(4.0f);
	UnitsText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	UnitsText->HorizSpacingAdjust = -2.5f;
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> TextComponent_Material(TEXT("Material'/Game/Materials/Font/TextMaterialWithTransparency.TextMaterialWithTransparency'"));
	UnitsText->SetTextMaterial(TextComponent_Material.Object);
	UnitsText->SetupAttachment(NumberHolder);

	// Set ship type
	PlanetShipType = ABaseShip::StaticClass();
}

void APlanet::BeginPlay()
{
	Super::BeginPlay();

	MISelectionMaterial = UMaterialInstanceDynamic::Create(SelectionMaterial, this);
	SelectionMesh->SetMaterial(0, MISelectionMaterial);
	ReceiveDeselect();

	// Set last grow to now
	TimeSinceLastGrow = GetWorld()->GetTimeSeconds();
}

void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Grow number of units
	if (PlanetOwner == nullptr)
	{
		// No grow
	}
	else
	{
		// Grow only if a second (or more) has passed since last grow
		if (GetWorld()->GetTimeSeconds() - TimeSinceLastGrow > 1.0f) {
			CurrentUnits += FMath::RoundToPositiveInfinity((float)CurrentUnits * GrowPercentPerSecond);
			CurrentUnits = FMath::Clamp(CurrentUnits, 0, MaxUnits);
			TimeSinceLastGrow = GetWorld()->GetTimeSeconds();
		}
	}

	UnitsText->SetText(FText::AsNumber(CurrentUnits));
}

void APlanet::ChangeSelectionColor(FLinearColor newColor)
{
	if (MISelectionMaterial)
		MISelectionMaterial->SetVectorParameterValue(FName("BaseColor"), newColor);
}

void APlanet::SetPlanetOwner(AActor * NewOwner)
{
	PlanetOwner = NewOwner;
	ReceiveDeselect();

	// Set new color for the orbit
	AActor* PlayerControllerAsActor = Cast<AActor>(GetWorld()->GetFirstPlayerController());
	if (NewOwner == nullptr)
		OrbitColor = ColorHoverNeutral.ToFColor(true);
	else if (NewOwner == PlayerControllerAsActor)
		OrbitColor = ColorHoverOwn.ToFColor(true);
	else
		OrbitColor = ColorHoverEnemy.ToFColor(true);
}

AActor * APlanet::GetPlanetOwner()
{
	return PlanetOwner;
}

void APlanet::SendUnitsToTarget(APlanet * TargetPlanet)
{
	if (TargetPlanet == this)
		return; // DO nothing if target is the same as source

	// Send half the units to the other planet
	int UnitsToSend = FMath::RoundToZero(CurrentUnits / 2.0f);
	for (int i = 0; i != UnitsToSend; i++)
	{
		// Spawn ship
		FVector InitialPosition = GetActorLocation();
		InitialPosition.X += FMath::RandRange(-ShipSpawnRange, ShipSpawnRange);
		InitialPosition.Y += FMath::RandRange(-ShipSpawnRange, ShipSpawnRange);

		FTransform ShipTransform(InitialPosition);
		ABaseShip* NewShip = GetWorld()->SpawnActorDeferred<ABaseShip>(PlanetShipType.Get(), ShipTransform);
		NewShip->InitializeShip(PlanetOwner, this, TargetPlanet);
		NewShip->FinishSpawning(ShipTransform);
	}
	CurrentUnits -= UnitsToSend;
}

void APlanet::ReceiveSelect()
{
	AActor* PlayerControllerAsActor = Cast<AActor>(GetWorld()->GetFirstPlayerController());
	if (PlayerControllerAsActor == PlanetOwner)
	{
		IsSelected = true;
		ChangeSelectionColor(ColorSelected);
	}
}

void APlanet::ReceiveDeselect()
{
	IsSelected = false;
	if (!IsHovered) {
		AActor* PlayerControllerAsActor = Cast<AActor>(GetWorld()->GetFirstPlayerController());
		if (PlanetOwner == nullptr)
			ChangeSelectionColor(ColorUnselectedNeutral);
		else if (PlanetOwner == PlayerControllerAsActor)
			ChangeSelectionColor(ColorUnselectedOwn);
		else
			ChangeSelectionColor(ColorUnselectedEnemy);
	}
	else {
		ReceiveHoverStart();
	}
}

void APlanet::ReceiveHoverStart()
{
	IsHovered = true;
	if (!IsSelected)
	{
		AActor* PlayerControllerAsActor = Cast<AActor>(GetWorld()->GetFirstPlayerController());
		if (PlanetOwner == nullptr)
			ChangeSelectionColor(ColorHoverNeutral);
		else if (PlanetOwner == PlayerControllerAsActor)
			ChangeSelectionColor(ColorHoverOwn);
		else
			ChangeSelectionColor(ColorHoverEnemy);
	}
}

void APlanet::ReceiveHoverEnd()
{
	IsHovered = false;
	if (!IsSelected)
	{
		ReceiveDeselect();
	}
}

bool APlanet::IsBeingHovered()
{
	return IsHovered;
}

void APlanet::ShipLanding(AActor * ShipOwner)
{
	// If both are from the same owner, unit count goes up.
	// Else unit count goes down. If reaches a negative value ship changes owner.
	if (PlanetOwner == ShipOwner)
	{
		CurrentUnits += 1;
	}
	else
	{
		CurrentUnits -= 1;
		if (CurrentUnits < 0) SetPlanetOwner(ShipOwner);
	}
}
