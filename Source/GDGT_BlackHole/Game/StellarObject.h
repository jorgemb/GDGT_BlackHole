// Copyright (c) 2019 Jorge Luis Martínez Bonilla

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StellarObject.generated.h"

UCLASS()
class GDGT_BLACKHOLE_API AStellarObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStellarObject();

	/** Default scene root */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;

	/** Static mesh that represents the object */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Mesh")
	class UStaticMeshComponent* ObjectMesh;

	/** Rotation speed in degrees per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement|Rotation")
	float RotationSpeed = 20.0f;

	/** Translation speed in degrees per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement|Translation")
	float TranslationSpeed = 1.0f;

	/** Object to orbit. If set to null it will remain static */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement|Translation")
	AActor* OrbitingObject = nullptr;

#if WITH_EDITOR
	/** Set debug color to draw */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	FColor OrbitColor = FColor::Magenta;

	/** Set if draw debug line */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool DrawOrbit = false;
#endif

	// Object selection
	/** Select object*/
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnSelect"))
	void ReceiveSelect();
	void ReceiveSelect_Implementation() {}

	/** Deselect object */
	UFUNCTION(BlueprintNativeEvent, meta=(DisplayName = "OnDeselect"))
	void ReceiveDeselect();
	void ReceiveDeselect_Implementation() {}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called on construction. Snaps to Z=0
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Contains the last rotation value */
	float LastRotation = 0.0f;

	/** Used to fix the translation distance to the other object */
	TSharedPtr<FVector> InitialTranslationFromObject;

	/** Containts the last translation value */
	float LastTranslation = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
