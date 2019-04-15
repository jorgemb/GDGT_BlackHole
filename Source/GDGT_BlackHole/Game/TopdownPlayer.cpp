// Copyright (c) 2019 Jorge Luis Martínez Bonilla

#include "TopdownPlayer.h"
#include <Components/SceneComponent.h>
#include <Components/InputComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/FloatingPawnMovement.h>
#include <Camera/CameraComponent.h>

// Sets default values
ATopdownPlayer::ATopdownPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make root scene component
	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	Root->SetupAttachment(RootComponent);

	// Create spring arm
	CameraBoon = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoon"));
	CameraBoon->SetupAttachment(Root);
	CameraBoon->SetRelativeLocation(FVector::ZeroVector);
	CameraBoon->SetRelativeRotation(FRotator(-90.0f, 0.0f, 90.0f));
	CameraBoon->TargetArmLength = 4000;
	CameraBoon->bDoCollisionTest = false;

	// Create camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(FName("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraBoon);

}

float ATopdownPlayer::GetZoom() const
{
	return CameraBoon->TargetArmLength;
}

void ATopdownPlayer::SetZoom(float newZoom)
{
	CameraBoon->TargetArmLength = newZoom;
}

// Called when the game starts or when spawned
void ATopdownPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ATopdownPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATopdownPlayer::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

