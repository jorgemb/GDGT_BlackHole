// Copyright (c) 2019 Jorge Luis Martínez Bonilla

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseShip.generated.h"

UCLASS()
class GDGT_BLACKHOLE_API ABaseShip : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseShip();

	/** Mesh for the ship */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Mesh")
	class UStaticMeshComponent* ShipMesh;

	/** Sets initial data for the ship */
	UFUNCTION(BlueprintCallable)
	virtual void InitializeShip(AActor* ShipOwner, class APlanet* SourcePlanet, class APlanet* TargetPlanet);

	/** Owner of the ship */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	AActor* ShipOwner = nullptr;

	// Movement data
	/** Speed for the ship */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status|Movement")
	float Speed = 300.0f;

	// Planet data
	/** Source planet of the ships */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status|Planets")
	class APlanet* SourcePlanet;

	/** Target planet of the ships */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status|Planets")
	class APlanet* TargetPlanet;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called on overlap with other actor
	UFUNCTION()
	virtual void OnShipBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
