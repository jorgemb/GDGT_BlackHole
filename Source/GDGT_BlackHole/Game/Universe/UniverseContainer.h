// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UniverseContainer.generated.h"

UCLASS()
class GDGT_BLACKHOLE_API AUniverseContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUniverseContainer();

	// Universe bounds
	/** Top left corner of the Universe */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Universe|Bounds")
	FVector TopLeftCorner;

	/** Bottom right corner of the Universe */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Universe|Bounds")
	FVector BottomRightCorner;

	/** Extra space after the bounds */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Universe|Bounds")
	float ExtraSpaceInBounds = 0;

	// Initialize
	/** Initialize universe to take into account the bounds of every object */
	UFUNCTION(BlueprintCallable, Category="Universe")
	void Initialize(float ExtraSpace);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
