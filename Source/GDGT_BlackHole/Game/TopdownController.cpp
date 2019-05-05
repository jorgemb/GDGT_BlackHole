// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#include "TopdownController.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <EngineUtils.h>
#include <ConstructorHelpers.h>
#include <Engine/World.h>
#include <Components/AudioComponent.h>
#include <Sound/SoundCue.h>
#include "Universe/Planet.h"
#include "Library/CollisionTrace.h"
#include "TopdownPlayer.h"

ATopdownController::ATopdownController() 
{
	// Create audio components
	SendShipsAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("SendShipsAudioComponent"));
	SendShipsAudioComponent->bAutoActivate = false;
	SendShipsAudioComponent->SetupAttachment(RootComponent);

	const ConstructorHelpers::FObjectFinder<USoundCue> SoundCue_SendShipsCue(TEXT("SoundCue'/Game/Audio/Effects/Spaceships/SendshipsCue.SendShipsCue'"));
	SendShipsSoundCue = SoundCue_SendShipsCue.Object;
}

void ATopdownController::BeginPlay() 
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	SetIgnoreMoveInput(true);

	// Get all planets in World
	for (TActorIterator<APlanet> PlanetIter(GetWorld()); PlanetIter; ++PlanetIter)
	{
		PlanetsInWorld.Add(*PlanetIter);
	}
}

void ATopdownController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		// Call hover events on planets
		TArray<APlanet *> HoveredPlanets = GetPlanetsUnderMouse();
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

void ATopdownController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (SendShipsSoundCue->IsValidLowLevel())
	{
		SendShipsAudioComponent->SetSound(SendShipsSoundCue);
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

void ATopdownController::SetPawnBounds(const FVector & TopLeftCorner, const FVector & BottomRightCorner)
{
	TopLeftBounds = TopLeftCorner;
	BottomRightBounds = BottomRightCorner;
}

TArray<APlanet*> ATopdownController::GetPlanetsUnderMouse(float TraceDistance)
{
	FVector MousePosition, MouseDirection;
	DeprojectMousePositionToWorld(MousePosition, MouseDirection);

	// Make trace to find planet under mouse
	FVector TraceStart = MousePosition;
	FVector TraceEnd = MousePosition + MouseDirection * TraceDistance;
	TArray<APlanet*> Planets = UCollisionTrace::LineTraceByActorType<APlanet>(
		GetWorld(),
		TraceStart,
		TraceEnd,
		FCollisionObjectQueryParams::AllDynamicObjects
		);

	return Planets;
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
	// Forward goes along the +X axis
	APawn* PlayerPawn = GetPawn();
	if (PlayerPawn)
	{
		float offset = value * CalculateMovementStep();
		FVector PawnLocation = PlayerPawn->GetActorLocation();
		float NewX = FMath::Clamp(PawnLocation.X + offset, TopLeftBounds.X, BottomRightBounds.X);
		PawnLocation.X = NewX;

		PlayerPawn->SetActorLocation(PawnLocation);
	}
}

void ATopdownController::MoveRight(float value)
{
	// Right goes along the +Y axis
	APawn* PlayerPawn = GetPawn();
	if (PlayerPawn)
	{
		float offset = value * CalculateMovementStep();
		FVector PawnLocation = PlayerPawn->GetActorLocation();
		float NewY = FMath::Clamp(PawnLocation.Y + offset, BottomRightBounds.Y, TopLeftBounds.Y);
		PawnLocation.Y = NewY;

		PlayerPawn->SetActorLocation(PawnLocation);
	}
}

void ATopdownController::SelectionStart()
{
	// Deselect every other object
	for (TActorIterator<AStellarObject> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->ReceiveDeselect();
	}

	// Select planets under mouse
	TArray<APlanet *> Planets = GetPlanetsUnderMouse();
	for (auto PlanetIter = Planets.CreateIterator(); PlanetIter; ++PlanetIter) 
	{
		(*PlanetIter)->ReceiveSelect();
	}
}

void ATopdownController::SelectionAdd()
{
	// Add planets selection under mouse
	TArray<APlanet *> Planets = GetPlanetsUnderMouse();
	for (auto PlanetIter = Planets.CreateIterator(); PlanetIter; ++PlanetIter)
	{
		(*PlanetIter)->ReceiveSelect();
	}
}

void ATopdownController::ExitLevel()
{
	FName MainMenuLevel(TEXT("/Game/Maps/MainMenu"));
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevel);
}

void ATopdownController::SendUnits()
{
	// Find if there is a planet under the mouse, and use only the first planet
	TArray<APlanet *> Planets = GetPlanetsUnderMouse();
	if (Planets.Num() > 0) {
		APlanet* TargetPlanet = Planets[0];
		for (TActorIterator<APlanet> PlanetIter(GetWorld()); PlanetIter; ++PlanetIter)
		{
			if (PlanetIter->GetIsSelected()) {
				PlanetIter->SendUnitsToTarget(TargetPlanet);
			}
		}
	}

	// Play sound for send ships
	SendShipsAudioComponent->Play();
}

// Called to bind functionality to input
void ATopdownController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind input events
	InputComponent->BindAction(FName("ZoomIn"), EInputEvent::IE_Pressed, this, &ATopdownController::InputZoomIn);
	InputComponent->BindAction(FName("ZoomOut"), EInputEvent::IE_Pressed, this, &ATopdownController::InputZoomOut);
	InputComponent->BindAction(FName("SelectionAdd"), EInputEvent::IE_Pressed, this, &ATopdownController::SelectionAdd);
	InputComponent->BindAction(FName("Selection"), EInputEvent::IE_Pressed, this, &ATopdownController::SelectionStart);
	InputComponent->BindAction(FName("SendUnits"), EInputEvent::IE_Pressed, this, &ATopdownController::SendUnits);
	InputComponent->BindAction(FName("Exit"), EInputEvent::IE_Pressed, this, &ATopdownController::ExitLevel);

	InputComponent->BindAxis(FName("MoveForward"), this, &ATopdownController::MoveForward);
	InputComponent->BindAxis(FName("MoveRight"), this, &ATopdownController::MoveRight);

}

