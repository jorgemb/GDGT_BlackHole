// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwningActorChanged, AActor*, NewOwner);

/**
 * Base class for UMG Widgets that belong to a single Actor in the world via a WidgetComponent, eg. for 3D health-bars, nameplate, interaction tooltip.
 * Based on the implementation in: https://www.tomlooman.com/creating-actor-widgets-umg/
 */
UCLASS()
class GDGT_BLACKHOLE_API UActorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/* Set the owning actor so widgets have access to whatere is, broacasting OnOwningActorChange event */
	UFUNCTION(BlueprintCallable, Category="Actor Widget|UI")
	void SetOwningActor(AActor* NewOwner);

	UPROPERTY(BlueprintAssignable, Category="Actor Widget|UI")
	FOnOwningActorChanged OnOwningActorChanged;

protected:
	/* Actor that widget is attached to via WidgetComponent */
	UPROPERTY(BlueprintReadOnly, Category="Actor Widget")
	AActor* OwningActor;
};
