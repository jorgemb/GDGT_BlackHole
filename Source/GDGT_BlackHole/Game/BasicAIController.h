// Copyright (c) 2019 Jorge Luis Martinez Bonilla

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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Universe")
	TArray<class APlanet*> PlanetsInWorld;

	/** Owned planets list */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Universe")
	TArray<class APlanet*> OwnedPlanets;

	/** Not owned planets list */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Universe")
	TArray<class APlanet*> NotOwnedPlanets;

	/** Get closest planet in the not owned list */
	UFUNCTION(BlueprintCallable, Category = "Universe")
	class APlanet* GetClosestNotOwnedPlanet(const FVector& Location) const;

protected:
	// Called when play begins
	void BeginPlay() override;

	// Called on every frame
	void Tick(float DeltaTime) override;
};
