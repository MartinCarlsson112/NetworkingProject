#pragma once
#include "CoreMinimal.h"
#include "NPMovementData.generated.h"

USTRUCT()
struct FNPMovementData
{
    GENERATED_BODY()

    FHitResult Hit;
    FVector FinalLocation = FVector::ZeroVector;
    FVector MovementDelta = FVector::ZeroVector;
    FVector StartLocation = FVector::ZeroVector;
};