// Copyright (c) 2019 Jorge Luis Martínez Bonilla

#include "StellarObject.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <ConstructorHelpers.h>
#include <Math/Matrix.h>
#include <DrawDebugHelpers.h>

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
	ObjectMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	ObjectMesh->SetupAttachment(Root);
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

	// Create gizmo
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
		SetActorTransform(FTransform(ObjectTransformation));
	}

	// Only do rotation
	LastRotation += RotationSpeed * DeltaTime;
	FQuat Rotation(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(LastRotation));
	SetActorRotation(Rotation);

#if WITH_EDITOR
	if (DrawOrbit) {
		// Draw point
		DrawDebugPoint(GetWorld(), GetActorLocation(), 3.0f, OrbitColor, false, 10.0f);
	}
#endif
}

