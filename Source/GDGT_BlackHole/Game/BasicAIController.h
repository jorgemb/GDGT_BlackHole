// Copyright (c) 2019 Jorge Luis Martínez Bonilla

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BasicAIController.generated.h"

/**
 * 
 */
UCLASS()
class GDGT_BLACKHOLE_API ABasicAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	/** Planet list */
	TArray<class APlanet*> PlanetsInWorld;

	/** Owned planets list */
	TArray<class APlanet*> OwnedPlanets;

protected:
	// Called when play begins
	void BeginPlay() override;

	// Called on every frame
	void Tick(float DeltaTime) override;
};
