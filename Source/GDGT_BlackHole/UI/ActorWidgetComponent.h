// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ActorWidgetComponent.generated.h"

/**
 * Extension of the WidgetComponent to make it easy to have owning Actor context to the Widget. Commonly used to display health bars, names, and interaction panels above Actors.
	Automatically calls SetOwningActor on the widget if the correct type of widget is used (ActorAttachWidget)
 * Based on the implementation in: https://www.tomlooman.com/creating-actor-widgets-umg/
 */
UCLASS()
class GDGT_BLACKHOLE_API UActorWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget() override;

	/** Default constructor */
	UActorWidgetComponent();
};
