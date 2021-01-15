#pragma once

#include "CoreMinimal.h"
#include "NPBaseProjectile.generated.h"

UCLASS(Blueprintable, BlueprintType)
class NETWORKINGPROJECT_API UNPBaseProjectile : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UNPBaseProjectile();

	UFUNCTION(BlueprintCallable)
	virtual void Fire(AActor* Shooter, const FVector& StartPosition, const FRotator& Direction, float Power, float DamageMultiplier);

};
