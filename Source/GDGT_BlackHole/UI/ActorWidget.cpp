// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#include "ActorWidget.h"

void UActorWidget::SetOwningActor(AActor* NewOwner) {
	if (OwningActor == NewOwner) {
		// Skip repeated calls
		return;
	}

	OwningActor = NewOwner;

	OnOwningActorChanged.Broadcast(NewOwner);
}

