// Copyright (c) 2019 Jorge Luis Martínez Bonilla

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TopdownPlayer.generated.h"

UCLASS()
class GDGT_BLACKHOLE_API ATopdownPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATopdownPlayer();

	/** Default scene root component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	class USceneComponent* Root;

	/** Camera boon that handles the zoom */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Camera")
	class USpringArmComponent* CameraBoon;

	/** Camera for the player */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Camera")
	class UCameraComponent* PlayerCamera;

	// Camera zoom
	/** Get current zoom for player */
	UFUNCTION(BlueprintCallable, Category = "Camera|Zoom")
	float GetZoom() const;

	/** Set current zoom for player */
	UFUNCTION(BlueprintCallable, Category = "Camera|Zoom")
	void SetZoom(float newZoom);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
