// Copyright (c) 2019 Jorge Luis Martínez Bonilla

#pragma once

#include "CoreMinimal.h"
#include "Game/StellarObject.h"
#include "Planet.generated.h"

/**
 * 
 */
UCLASS()
class GDGT_BLACKHOLE_API APlanet : public AStellarObject
{
	GENERATED_BODY()

public:
	/** Default constructor */
	APlanet();

	/** Mesh to display current selection */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Selection")
	class UStaticMeshComponent* SelectionMesh;

	/** Spring arm for handling the number count */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Status")
	class USpringArmComponent* NumberHolder;

	/** Text for number of units */ 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Status")
	class UTextRenderComponent* UnitsText;

	/** Returns if the planet is selected */
	UFUNCTION(BlueprintGetter)
	bool GetIsSelected() const
	{
		return IsSelected;
	}

	/** Owner of the planet */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	AActor* PlanetOwner = nullptr;

	// Selection colors
	/** Color to use when a unit is selected and is owned by the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Selection")
	FLinearColor ColorSelected;
	/** Color to use when hovering the mouse over an owned planet */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Selection")
	FLinearColor ColorHoverOwn;
	/** Color to use when hovering the mouse over an enemy planet */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Selection")
	FLinearColor ColorHoverEnemy;
	/** Color to use when hovering the mouse over a neutral planet */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Selection")
	FLinearColor ColorHoverNeutral;
	/** Color to use when a planet is deselected but owned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Selection")
	FLinearColor ColorUnselected;

	// Units
	/** Number of units the planet currently has*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status|Units")
	int32 CurrentUnits = 50;

	/** Type of ship to spawn from this planet */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status|Units")
	TSubclassOf<class ABaseShip> PlanetShipType;

	// Events
	/** Selection functions */
	virtual void ReceiveSelect() override;
	virtual void ReceiveDeselect() override;

	/** Hover functions */
	virtual void ReceiveHoverStart() override;
	virtual void ReceiveHoverEnd() override;
	virtual bool IsBeingHovered() override;
protected:
	/** Begin play event */
	virtual void BeginPlay() override;

	/** Tick event */
	virtual void Tick(float DeltaTime) override;

	/** Save if the planet is selected */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter=GetIsSelected)
	bool IsSelected = false;

	/** Save if the planet is being hovered */
	bool IsHovered = false;
	
	/** Material for selection */
	class UMaterialInterface* SelectionMaterial;
	class UMaterialInstanceDynamic* MISelectionMaterial;

	/** Sets the color for the selection material */
	void ChangeSelectionColor(FLinearColor newColor);

};
