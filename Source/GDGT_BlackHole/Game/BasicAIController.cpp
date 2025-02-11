// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#include "BasicAIController.h"
#include <EngineUtils.h>
#include "Game/Universe/Planet.h"

APlanet * ABasicAIController::GetClosestNotOwnedPlanet(const FVector & Location) const
{
	float CurrentMinDistance = INFINITY;
	APlanet* CurrentClosestPlanet = nullptr;
	for (auto CurrentPlanet = NotOwnedPlanets.CreateConstIterator(); CurrentPlanet; ++CurrentPlanet)
	{
		float SquaredDistance = FVector::DistSquared((*CurrentPlanet)->GetActorLocation(), Location);
		if (SquaredDistance < CurrentMinDistance)
		{
			CurrentMinDistance = SquaredDistance;
			CurrentClosestPlanet = (*CurrentPlanet);
		}
	}

	return CurrentClosestPlanet;
}

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
	NotOwnedPlanets.Empty();
	AActor* ControllerAsActor = Cast<AActor>(this);
	for (auto PlanetIter = PlanetsInWorld.CreateIterator(); PlanetIter; ++PlanetIter)
	{
		APlanet* CurrentPlanet = *PlanetIter;
		if (CurrentPlanet->GetPlanetOwner() == ControllerAsActor)
			OwnedPlanets.Add(CurrentPlanet);
		else
			NotOwnedPlanets.Add(CurrentPlanet);
	}
}
