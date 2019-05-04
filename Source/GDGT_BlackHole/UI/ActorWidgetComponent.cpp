// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#include "ActorWidgetComponent.h"
#include "UI/ActorWidget.h"
#include "GDGT_BlackHole.h"

UActorWidgetComponent::UActorWidgetComponent()
{
	// Set comong defaults when using widgets on actors
	SetDrawAtDesiredSize(true);
	SetWidgetSpace(EWidgetSpace::Screen);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UActorWidgetComponent::InitWidget()
{
	// Base implementation creates the "Widget" instance
	Super::InitWidget();

	if (Widget)
	{
#if !UE_BUILD_SHIPPING
		if (!Widget->IsA(UActorWidget::StaticClass()))
		{
			// Suggest deriving from actor widget (not strictly required, but you lose the benefit of auto-assigning the Actor this way)
			UE_LOG(LogGame, Warning, TEXT("WidgetClass for %s does not derive from SActorWidget"), *GetNameSafe(GetOwner()));
		}
#endif

		UActorWidget* WidgetInst = Cast<UActorWidget>(Widget);
		if (WidgetInst)
		{
			// Assign the owner
			WidgetInst->SetOwningActor(GetOwner());
		}
	}
}
