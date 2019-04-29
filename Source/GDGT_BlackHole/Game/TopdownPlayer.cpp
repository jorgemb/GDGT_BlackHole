// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#include "TopdownPlayer.h"
#include <Components/SceneComponent.h>
#include <Components/InputComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/FloatingPawnMovement.h>
#include <Camera/CameraComponent.h>
#include <ConstructorHelpers.h>
#include <Matrix.h>

#include <DrawDebugHelpers.h>

// Sets default values
ATopdownPlayer::ATopdownPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make root scene component
	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;

	// Create spring arm
	CameraBoon = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoon"));
	CameraBoon->SetupAttachment(Root);
	CameraBoon->SetRelativeLocation(FVector::ZeroVector);
	CameraBoon->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	CameraBoon->TargetArmLength = 8000;
	CameraBoon->bDoCollisionTest = false;

	// Create camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(FName("PlayerCamera"));
	PlayerCamera->PostProcessSettings.bOverride_AutoExposureMinBrightness = true;
	PlayerCamera->PostProcessSettings.bOverride_AutoExposureMaxBrightness = true;
	PlayerCamera->PostProcessSettings.AutoExposureMinBrightness = 1.0f;
	PlayerCamera->PostProcessSettings.AutoExposureMinBrightness = 1.0f;
	PlayerCamera->SetupAttachment(CameraBoon);

	// Create skybox
	Skybox = CreateDefaultSubobject<UStaticMeshComponent>(FName("Skybox"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> Skybox_Mesh(TEXT("StaticMesh'/Engine/EngineSky/SM_SkySphere.SM_SkySphere'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> Skybox_Material(TEXT("Material'/Game/Materials/SkyboxMaterial.SkyboxMaterial'"));
	Skybox->SetStaticMesh(Skybox_Mesh.Object);
	Skybox->SetMaterial(0, Skybox_Material.Object);
	Skybox->SetupAttachment(Root);
	Skybox->SetRelativeLocation(FVector(0.0f, 0.0f, 0000.0f));
	Skybox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float ATopdownPlayer::GetZoom() const
{
	return CameraBoon->TargetArmLength;
}

void ATopdownPlayer::SetZoom(float newZoom)
{
	if (FMath::IsNearlyEqual(newZoom, CameraBoon->TargetArmLength))
		return;

	ZoomStartTime = ZoomCurrentTime = GetWorld()->GetTimeSeconds();
	ZoomStartingOffset = CameraBoon->TargetArmLength;
	ZoomTargetOffset = newZoom;
	IsZooming = true;
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

	// Do zooming animation
	if (IsZooming) {
		ZoomCurrentTime += DeltaTime;
		float LerpAlpha = (ZoomCurrentTime - ZoomStartTime) / ZoomSpeed;
		if (LerpAlpha < 1.0f)
		{
			CameraBoon->TargetArmLength = FMath::Lerp(ZoomStartingOffset, ZoomTargetOffset, LerpAlpha);
		}
		else
		{
			CameraBoon->TargetArmLength = ZoomTargetOffset;
			IsZooming = false;
		}
	}

#if WITH_EDITOR
	// Draw pawn
	DrawDebugCircle(GetWorld(), FTranslationMatrix(GetActorLocation()), 100.0f, 50, FColor::Cyan);
#endif
}

void ATopdownPlayer::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

