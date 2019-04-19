// Copyright (c) 2019 Jorge Luis Martínez Bonilla

#include "Planet.h"
#include <Components/StaticMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/TextRenderComponent.h>
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
	ColorSelected = ColorHoverOwn = ColorUnselected = FColor(0, 0, 128, 255);
	ColorHoverOwn.A = 0.7f;
	ColorUnselected.A = 0.3f;
	ColorHoverEnemy = FColor(128, 0, 0, 128);
	ColorHoverNeutral = FColor(128, 128, 128, 128);

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
}

void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UnitsText->SetText(FString::FromInt(CurrentUnits));
}

void APlanet::ChangeSelectionColor(FLinearColor newColor)
{
	MISelectionMaterial->SetVectorParameterValue(FName("BaseColor"), newColor);
}

void APlanet::ReceiveSelect()
{
	IsSelected = true;
	ChangeSelectionColor(ColorSelected);
}

void APlanet::ReceiveDeselect()
{
	IsSelected = false;
	ChangeSelectionColor(ColorUnselected);
}

void APlanet::ReceiveHoverStart()
{
	IsHovered = true;
	if (!IsSelected)
	{
		ChangeSelectionColor(ColorHoverNeutral);
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
