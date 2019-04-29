// Copyright (c) 2019 Jorge Luis Martinez Bonilla

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <CollisionQueryParams.h>
#include <Engine/World.h>
#include "CollisionTrace.generated.h"

/**
 * Helper functions for retrieving a list of objects from a collision trace.
 */
UCLASS()
class GDGT_BLACKHOLE_API UCollisionTrace : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
protected:

public:
	/** Retrieve all objects of the given type that intersect a trace */
	template<typename T>
	static TArray<T*> LineTraceByActorType(UWorld* World, const FVector& vStart, const FVector& vEnd,
		const FCollisionObjectQueryParams& ObjectQueryParams, const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam) {
		// Make the line trace
		TArray<FHitResult> Hits;
		bool GotHits = World->LineTraceMultiByObjectType(
			Hits,
			vStart,
			vEnd,
			ObjectQueryParams,
			Params
		);

		// Filter by type
		TArray<T*> HitObjects;
		if (GotHits) {
			for(auto hit = Hits.CreateIterator(); hit; ++hit)
			{
				if (hit->Actor->IsA(T::StaticClass())) {
					HitObjects.Add(Cast<T>(hit->Actor.Get()));
				}
			}
		}

		return HitObjects;
	}
};
