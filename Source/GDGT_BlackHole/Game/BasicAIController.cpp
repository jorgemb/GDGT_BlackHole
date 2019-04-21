// Copyright (c) 2019 Jorge Luis Martínez Bonilla

#include "BasicAIController.h"
#include <EngineUtils.h>
#include "Game/Universe/Planet.h"

void ABasicAIController::BeginPlay()
{
	Super::BeginPlay();

	// Get all planets in the world
	for (TActorIterator<APlanet> PlanetIter(GetWorld()); PlanetIter; ++PlanetIter)
	{
		PlanetsInWorld.Add(*PlanetIter);
	}
}

void ABasicAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update list of owned planets
	OwnedPlanets.Empty();
	AActor* ControllerAsActor = Cast<AActor>(this);
	for (auto PlanetIter = PlanetsInWorld.CreateIterator(); PlanetIter; ++PlanetIter)
	{
		APlanet* CurrentPlanet = *PlanetIter;
		if (CurrentPlanet->GetPlanetOwner() == ControllerAsActor) OwnedPlanets.Add(CurrentPlanet);
	}
}
