#pragma once
#include "CoreMinimal.h"


struct FNPMovementData
{
    FHitResult Hit;
    FVector FinalLocation = FVector::ZeroVector;
    FVector MovementDelta = FVector::ZeroVector;
    FVector StartLocation = FVector::ZeroVector;
}