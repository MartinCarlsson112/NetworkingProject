#pragma once

#include "CoreMinimal.h"
#include "NPBaseProjectile.h"
#include "../Components/NPProjectileMovementComponent.h"
#include "Engine/EngineTypes.h"
#include "NPArrowProjectile.generated.h"

UCLASS(Blueprintable, BlueprintType)
class NETWORKINGPROJECT_API UNPArrowProjectile : public UNPBaseProjectile
{
	GENERATED_BODY()
public:
	UNPArrowProjectile();
	void Fire(AActor* Shooter, const FVector& StartPosition, const FRotator& Direction, float Power, float DamageMultiplier) override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnCollision(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Expire();

	UFUNCTION(BlueprintCallable)
	void SetTeam(int TeamIndex);

	UFUNCTION(BlueprintCallable)
	int GetTeam()const;

protected:
	void BeginPlay() override;

	FVector InitialDirection;
	FVector CurrentVelocity;
	bool HasBeenFired;

	UPROPERTY()
	int TeamIndex = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	float ArrowLifetime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	float ArrowHitLifetime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Shooting)
	float ArrowDamage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	float CurrentDamage;

	FTimerHandle LifetimeTimerHandle;
};
