// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#include "StellarObject.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>
#include <ConstructorHelpers.h>
#include <Math/Matrix.h>
#include <DrawDebugHelpers.h>
#include <SceneManagement.h>

// Sets default values
AStellarObject::AStellarObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create scene component as root
	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;

	// Create static mesh
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ObjectMesh"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> SphereObject(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	ObjectMesh->SetStaticMesh(SphereObject.Object);
	ObjectMesh->SetCollisionProfileName(FName("BlockAllDynamic"));
	ObjectMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	ObjectMesh->SetupAttachment(Root);
}

void AStellarObject::OnDrawOrbit()
{
	// Draw the orbit
	if (DrawOrbit) {
		if (OrbitingObject) {
			DrawCircle(
				GetWorld(),
				OrbitingObject->GetActorLocation(),
				FVector::ForwardVector,
				FVector::RightVector,
				OrbitColor,
				FVector::Distance(GetActorLocation(), OrbitingObject->GetActorLocation()),
				100,
				false,
				-1.0f,
				(uint8)'\000',
				OrbitThickness
			);
		}
	}
}

FVector AStellarObject::GetTopLeftBounds()
{
	if (OrbitingObject)
	{
		FVector TopLeftBounds = OrbitingObject->GetTopLeftBounds();
		float Distance = (GetActorLocation() - OrbitingObject->GetActorLocation()).Size();
		TopLeftBounds.X -= Distance;
		TopLeftBounds.Y += Distance;
		return TopLeftBounds;
	}
	else
	{
		return GetActorLocation();
	}
}

FVector AStellarObject::GetBottomRightBounds()
{
	if (OrbitingObject)
	{
		FVector BottomRightBounds = OrbitingObject->GetBottomRightBounds();
		float Distance = (GetActorLocation() - OrbitingObject->GetActorLocation()).Size();
		BottomRightBounds.X += Distance;
		BottomRightBounds.Y -= Distance;
		return BottomRightBounds;
	}
	else
	{
		return GetActorLocation();
	}
}

// Called when the game starts or when spawned
void AStellarObject::BeginPlay()
{
	Super::BeginPlay();
}

void AStellarObject::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Snap to Z=0
	FVector position(GetActorLocation());
	position.Z = 0.0f;
	SetActorLocation(position);

#if WITH_EDITOR
	// Create gizmo
	if (OrbitingObject) {
		DrawDebugCircle(GetWorld(), FRotationTranslationMatrix(FRotator(-90.0f, 0.0f, 0.0f), OrbitingObject->GetActorLocation()),
			(GetActorLocation() - OrbitingObject->GetActorLocation()).Size(), 200, FColor::White, false, 1.0f);
	}
#endif
}

// Called every frame
void AStellarObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Make translation motion
	if (OrbitingObject) {
		// Get initial distance
		if (!InitialTranslationFromObject.IsValid())
			InitialTranslationFromObject = TSharedPtr<FVector>(new FVector(GetActorLocation() - OrbitingObject->GetActorLocation()));

		// Get starting matrix
		FMatrix ObjectTransformation(FMatrix::Identity);

		// Get the distance from the other object
		ObjectTransformation *= FTranslationMatrix(*InitialTranslationFromObject);

		// Get the rotation for the orbit
		LastTranslation += TranslationSpeed * DeltaTime;
		ObjectTransformation *= FRotationMatrix(FRotator(0.0f, LastTranslation, 0.0f));

		// Get other object translation
		ObjectTransformation *= FTranslationMatrix(OrbitingObject->GetActorLocation());

		// Scale the object
		FVector ObjectScale = GetActorScale();
		SetActorTransform(FTransform(ObjectTransformation));
		SetActorScale3D(ObjectScale);
	}

	// Only do rotation
	LastRotation += RotationSpeed * DeltaTime;
	FQuat Rotation(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(LastRotation));
	SetActorRotation(Rotation);

	// Draw orbit
	if(DrawOrbit) OnDrawOrbit();
}

