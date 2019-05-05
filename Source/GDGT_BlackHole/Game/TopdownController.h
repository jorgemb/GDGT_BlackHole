// Copyright (c) 2019 Jorge Luis Martinez Bonilla

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
	ATopdownController();

	// Events
	/** Begin play event */
	virtual void BeginPlay() override;

	/** Tick event */
	virtual void Tick(float DeltaTime) override;

	/** Post initialize components */
	virtual void PostInitializeComponents() override;

	// Zoom options
	/** Current zoom for the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera|Zoom")
	float ZoomCurrent = 8000;

	/** Maximum zoom for the camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
	float ZoomMax = 18000;

	/** Minimum zoom for the camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
	float ZoomMin = 2000;

	/** Zoom step when pressing buttons */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
	float ZoomStep = 1000;

	// Changes zoom in player pawn
	void ChangePlayerZoom(float value);

	// Movement options
	/** Movement step on maximum zoom */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementStepMin = 50;

	/** Movement step on minimum zoom */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementStepMax = 200;

	// Planets in the world
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game Data")
	TArray<class APlanet*> PlanetsInWorld;

	/** Checks if the pawn is inside of the bounds, and fixes it if it's outsite */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetPawnBounds(const FVector& TopLeftCorner, const FVector& BottomRightCorner);

	// Audio options
	/** Audio component for playing sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Audio")
	class UAudioComponent* SendShipsAudioComponent;

	/** Sound cue for sending ships */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Audio")
	class USoundCue* SendShipsSoundCue;

protected:
	// Returns all the planets under the mouse
	TArray<class APlanet*> GetPlanetsUnderMouse(float TraceDistance = 200000);

	// Pawn bounds
	FVector TopLeftBounds, BottomRightBounds;

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

	// Called when player wants to exit
	void ExitLevel();

	// Called when player wants to send units to other planet
	void SendUnits();

	// Create input component
	void SetupInputComponent() override;
};
