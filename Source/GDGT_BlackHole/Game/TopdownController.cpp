// Copyright (c) 2019 Jorge Luis Martínez Bonilla

#include "TopdownController.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <EngineUtils.h>
#include <Engine/World.h>
#include "Universe/Planet.h"
#include "Library/CollisionTrace.h"
#include "TopdownPlayer.h"

void ATopdownController::BeginPlay() 
{
	bShowMouseCursor = true;
}

void ATopdownController::Tick(float DeltaTime)
{
	// Get mouse position
	float MouseX, MouseY;
	bool GotMouse = GetMousePosition(MouseX, MouseY);
	if (GotMouse) {
		int ViewportX, ViewportY;
		GetViewportSize(ViewportX, ViewportY);

		float MouseRelativeX = MouseX / ViewportX;
		float MouseRelativeY = MouseY / ViewportY;

		// Calculate if movement is done
		const float Bounds = 0.1f;
		const float Conversion = 1.0f / Bounds;
		float RangeX = FMath::Abs(MouseRelativeX - 0.5f) - (0.5f - Bounds);
		float RangeY = FMath::Abs(MouseRelativeY - 0.5f) - (0.5f - Bounds);
		float FixRange = (RangeX > 0 && RangeY > 0) ? FMath::Sin(45) : 1.0f;

		if (RangeX > 0.0f) // Movement in X
			MoveRight(RangeX * Conversion * FMath::Sign(MouseRelativeX - 0.5f) * FixRange);
		if (RangeY > 0.0f) // Movent in Y
			MoveForward(RangeY * Conversion * -FMath::Sign(MouseRelativeY - 0.5f) * FixRange);
	}

	// Calculate hover for all the planets
	FVector MouseLocation, MouseDirection;
	bool GotMouseProjection = DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	if (GotMouseProjection)
	{
		// Get hits under mouse
		float LargeNumber = 20000;
		FVector MouseRayStart = MouseLocation;
		FVector MouseRayEnd = MouseLocation + MouseDirection * LargeNumber;
		TArray<AStellarObject*> HoveredPlanets = UCollisionTrace::LineTraceByActorType<AStellarObject>(
			GetWorld(),
			MouseRayStart,
			MouseRayEnd,
			FCollisionObjectQueryParams::AllDynamicObjects
			);

		for (TActorIterator<AStellarObject> ObjIter(GetWorld()); ObjIter; ++ObjIter)
		{
			if (HoveredPlanets.Contains(*ObjIter))
			{
				if (!ObjIter->IsBeingHovered()) ObjIter->ReceiveHoverStart();
			}
			else
			{
				if (ObjIter->IsBeingHovered()) ObjIter->ReceiveHoverEnd();
			}
		}
	}
}

void ATopdownController::ChangePlayerZoom(float value)
{
	ATopdownPlayer* PlayerPawn = Cast<ATopdownPlayer>(GetPawn());

	if (PlayerPawn) {
		PlayerPawn->SetZoom(value);
		ZoomCurrent = PlayerPawn->GetZoom();
	}
}

void ATopdownController::InputZoomIn()
{
	ChangePlayerZoom(FMath::Clamp(ZoomCurrent - ZoomStep, ZoomMin, ZoomMax));
}

void ATopdownController::InputZoomOut()
{
	ChangePlayerZoom(FMath::Clamp(ZoomCurrent + ZoomStep, ZoomMin, ZoomMax));
}

float ATopdownController::CalculateMovementStep()
{
	ATopdownPlayer* PlayerPawn = Cast<ATopdownPlayer>(GetPawn());
	if (PlayerPawn)
		ZoomCurrent = PlayerPawn->GetZoom();

	return FMath::GetMappedRangeValueClamped(FVector2D(ZoomMin, ZoomMax), FVector2D(MovementStepMin, MovementStepMax), ZoomCurrent);
}

void ATopdownController::MoveForward(float value)
{
	APawn* PlayerPawn = GetPawn();
	if (PlayerPawn) {
		PlayerPawn->AddActorLocalOffset(FVector(1.0f, 0.0f, 0.0f) * value * CalculateMovementStep());
	}
}

void ATopdownController::MoveRight(float value)
{
	APawn* PlayerPawn = GetPawn();
	if (PlayerPawn) {
		PlayerPawn->AddActorLocalOffset(FVector(0.0f, 1.0f, 0.0f) * value * CalculateMovementStep());
	}
}

void ATopdownController::SelectionStart()
{
	FVector MousePosition, MouseDirection;
	DeprojectMousePositionToWorld(MousePosition, MouseDirection);

	// Deselect every other object
	for (TActorIterator<AStellarObject> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->ReceiveDeselect();
	}

	// Make trace to find planet under mouse
	FVector TraceStart = MousePosition;
	FVector TraceEnd = MousePosition + MouseDirection * 20000;
	TArray<APlanet*> Planets = UCollisionTrace::LineTraceByActorType<APlanet>(
		GetWorld(),
		TraceStart,
		TraceEnd,
		FCollisionObjectQueryParams::AllDynamicObjects
		);

	for (auto PlanetIter = Planets.CreateIterator(); PlanetIter; ++PlanetIter) {
		(*PlanetIter)->ReceiveSelect();
	}
}

// Called to bind functionality to input
void ATopdownController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind input events
	InputComponent->BindAction(FName("ZoomIn"), EInputEvent::IE_Pressed, this, &ATopdownController::InputZoomIn);
	InputComponent->BindAction(FName("ZoomOut"), EInputEvent::IE_Pressed, this, &ATopdownController::InputZoomOut);
	InputComponent->BindAction(FName("Selection"), EInputEvent::IE_Pressed, this, &ATopdownController::SelectionStart);

	InputComponent->BindAxis(FName("MoveForward"), this, &ATopdownController::MoveForward);
	InputComponent->BindAxis(FName("MoveRight"), this, &ATopdownController::MoveRight);
}

