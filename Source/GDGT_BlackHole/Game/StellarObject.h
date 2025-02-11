// Copyright (c) 2019 Jorge Luis Martinez Bonilla

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StellarObject|Movement|Rotation")
	float RotationSpeed = 20.0f;

	/** Translation speed in degrees per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StellarObject|Movement|Translation")
	float TranslationSpeed = 1.0f;

	/** Object to orbit. If set to null it will remain static */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StellarObject|Movement|Translation")
	AStellarObject* OrbitingObject = nullptr;

	/** Set if draw line for orbit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StellarObject|Movement|Translation")
	bool DrawOrbit = true;

	/** Set if draw line for orbit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StellarObject|Movement|Translation")
	FColor OrbitColor = FColor::White;

	/** Set orbit thickness */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StellarObject|Movement|Translation")
	float OrbitThickness = 20.0f;

	/** Called when the orbit is going to be drawn */
	virtual void OnDrawOrbit();

	// Object selection
	/** Select object*/
	UFUNCTION(BlueprintCallable, meta=(DisplayName = "OnSelect"))
	virtual void ReceiveSelect() {};

	/** Deselect object */
	UFUNCTION(BlueprintCallable, meta=(DisplayName = "OnDeselect"))
	virtual void ReceiveDeselect() {};

	/** Hover on object */
	UFUNCTION(BlueprintCallable, meta=(DisplayName = "OnHoverStart"))
	virtual void ReceiveHoverStart() {};

	/** End hover on object */
	UFUNCTION(BlueprintCallable, meta=(DisplayName = "OnHoverEnd"))
	virtual void ReceiveHoverEnd() {};

	/** Check if object is being hovered */
	UFUNCTION(BlueprintCallable)
	virtual bool IsBeingHovered() { return false; }

	// Object bounds
	/** Return top left corner of orbit bounds */
	UFUNCTION(BlueprintCallable, Category="Bounds")
	virtual FVector GetTopLeftBounds();

	/** Return bottom right corner of orbit bounds */
	UFUNCTION(BlueprintCallable, Category="Bounds")
	virtual FVector GetBottomRightBounds();

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
