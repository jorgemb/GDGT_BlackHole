// Copyright (c) 2019 Jorge Luis Martinez Bonilla

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
	virtual void InitializeShip(AActor* OwnerActor, class APlanet* SourcePlanet, class APlanet* TargetPlanet);

	/** Owner of the ship */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	AActor* ShipOwner = nullptr;

	/** Color to use for own ships */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Colors")
	FLinearColor ColorOwn;
	/** Color to use for enemy ships */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Colors")
	FLinearColor ColorEnemy;

	// Movement data
	/** Speed for the ship */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status|Movement")
	float Speed = 400.0f;

	/** On spawn ship speed variation. This means the the speed will be added a random value between [-SpeedVariation, SpeedVariation] */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Status|Movement")
	float SpeedVariationOnSpawn = 250.0f;

	/** Random speed variation to add every second after initial seconds. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Status|Movement")
	float SpeedVariationOnTime = 100.0f;

	/** Time after which acceleration will start */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Status|Movement")
	float SecondsToStartAcceleration = 1.0f;

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

	// Material to use for the Ship
	class UMaterialInterface* ShipMaterial;
	class UMaterialInstanceDynamic* MIShipMaterial;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
