#pragma once

#include "CoreMinimal.h"
#include "NPBaseProjectile.generated.h"

class NETWORKINGPROJECT_API ANP_Player;

UCLASS(Blueprintable, BlueprintType)
class NETWORKINGPROJECT_API UNPBaseProjectile : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UNPBaseProjectile();
	
	void Initialize(const int ProjectileIndex, ANP_Player* PlayerRef);

	UFUNCTION(BlueprintCallable)
	virtual void Fire(const FVector& StartPosition, const FRotator& Direction, float Power, float DamageMultiplier);
	
	UFUNCTION(BlueprintCallable)
	virtual void ApplyCorrection(const FVector& Direction);
	UFUNCTION()
	virtual	void Expire();
	virtual void ActivateProjectile();
	virtual void DeactivateProjectile();

	void ReturnArrow();


private:

	int Index;
	ANP_Player* Player;

};
