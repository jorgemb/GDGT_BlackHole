// Copyright (c) 2019 Jorge Luis Martínez Bonilla

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopdownController.generated.h"

/**
 * 
 */
UCLASS()
class GDGT_BLACKHOLE_API ATopdownController : public APlayerController
{
	GENERATED_BODY()

public:
	// Events
	/** Begin play event */
	virtual void BeginPlay() override;

	/** Tick event */
	virtual void Tick(float DeltaTime) override;

	// Zoom options
	/** Current zoom for the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Zoom")
	float ZoomCurrent = 8000;

	/** Maximum zoom for the camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
	float ZoomMax = 12000;

	/** Minimum zoom for the camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
	float ZoomMin = 1600;

	/** Zoom step when pressing buttons */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
	float ZoomStep = 800;

	// Changes zoom in player pawn
	void ChangePlayerZoom(float value);

	// Movement options
	/** Movement step on maximum zoom */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementStepMin = 50;

	/** Movement step on minimum zoom */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementStepMax = 200;

protected:
	// Returns all the planets under the mouse
	TArray<class APlanet*> GetPlanetsUnderMouse(float TraceDistance = 20000);

	// Called on ZoomIn
	void InputZoomIn();

	// Called on ZoomOut
	void InputZoomOut();

	// Calculates movement step
	float CalculateMovementStep();

	// Called on axis forward movement
	void MoveForward(float value);

	// Called on axis right movement
	void MoveRight(float value);

	// Called for selection
	void SelectionStart();
	void SelectionAdd();

	// Called when player wants to send units to other planet
	void SendUnits();

	// Planets in the world
	TArray<class APlanet*> PlanetsInWorld;

	// Create input component
	void SetupInputComponent() override;
};
