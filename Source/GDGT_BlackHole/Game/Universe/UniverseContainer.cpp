// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#include "UniverseContainer.h"
#include <EngineUtils.h>
#include <DrawDebugHelpers.h>
#include "Game/Universe/Planet.h"

// Sets default values
AUniverseContainer::AUniverseContainer()
{
#if WITH_EDITOR
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#else
	PrimaryActorTick.bCanEverTick = false;
#endif

}

// Called when the game starts or when spawned
void AUniverseContainer::Initialize(float ExtraSpace)
{
	// Calculate the size of the universe
	bool FirstObject = true;
	for (TActorIterator<AStellarObject> ObjectIter(GetWorld()); ObjectIter; ++ObjectIter)
	{
		//FVector CurrentLocation = ObjectIter->GetActorLocation();

		//// If is the first object to look for use its location as max and min
		//if (FirstObject)
		//{
		//	TopLeftCorner = CurrentLocation;
		//	BottomRightCorner = CurrentLocation;
		//	FirstObject = false;
		//}

		//// Calculate max and min bounds for the object
		//if (ObjectIter->OrbitingObject != nullptr)
		//{
		//	// Object is orbitting another
		//	FVector Center = ObjectIter->OrbitingObject->GetActorLocation();
		//	float DistanceToCenter = (Center - ObjectIter->GetActorLocation()).Size();

		//	// Get the maximum and minimum bounds of the orbit
		//	TopLeftCorner.X = FMath::Min(TopLeftCorner.X, Center.X - DistanceToCenter);
		//	TopLeftCorner.Y = FMath::Max(TopLeftCorner.Y, Center.Y + DistanceToCenter);
		//	BottomRightCorner.X = FMath::Max(BottomRightCorner.X, Center.X + DistanceToCenter);
		//	BottomRightCorner.Y = FMath::Min(BottomRightCorner.Y, Center.Y - DistanceToCenter);
		//}
		//else
		//{
		//	// Object is static
		//	TopLeftCorner.X = FMath::Min(TopLeftCorner.X, CurrentLocation.X);
		//	TopLeftCorner.Y = FMath::Max(TopLeftCorner.Y, CurrentLocation.Y);
		//	BottomRightCorner.X = FMath::Max(BottomRightCorner.X, CurrentLocation.X);
		//	BottomRightCorner.Y = FMath::Min(BottomRightCorner.Y, CurrentLocation.Y);
		//}

		// Add the bounds of every stellar object
		FVector ObjectTopLeftCorner = ObjectIter->GetTopLeftBounds();
		FVector ObjectBottomRightCorner = ObjectIter->GetBottomRightBounds();
		TopLeftCorner.X = FMath::Min(TopLeftCorner.X, ObjectTopLeftCorner.X);
		TopLeftCorner.Y = FMath::Max(TopLeftCorner.Y, ObjectTopLeftCorner.Y);
		BottomRightCorner.X = FMath::Max(BottomRightCorner.X, ObjectBottomRightCorner.X);
		BottomRightCorner.Y = FMath::Min(BottomRightCorner.Y, ObjectBottomRightCorner.Y);
	}


	// Add extra space to bounds
	ExtraSpaceInBounds = ExtraSpace;
	TopLeftCorner.X -= ExtraSpaceInBounds;
	TopLeftCorner.Y += ExtraSpaceInBounds;
	TopLeftCorner.Z = 0;

	BottomRightCorner.X += ExtraSpaceInBounds;
	BottomRightCorner.Y -= ExtraSpaceInBounds;
	BottomRightCorner.Z = 0;
}

// Called every frame
void AUniverseContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR
	FVector TopRightCorner = FVector(BottomRightCorner.X, TopLeftCorner.Y, 0);
	FVector BottomLeftCorner = FVector(TopLeftCorner.X, BottomRightCorner.Y, 0);
	DrawDebugLine(GetWorld(), TopRightCorner, TopLeftCorner, FColor::Magenta);
	DrawDebugLine(GetWorld(), TopLeftCorner, BottomLeftCorner, FColor::Magenta);
	DrawDebugLine(GetWorld(), BottomLeftCorner, BottomRightCorner, FColor::Magenta);
	DrawDebugLine(GetWorld(), BottomRightCorner, TopRightCorner, FColor::Magenta);
#endif
}

