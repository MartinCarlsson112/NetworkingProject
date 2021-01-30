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
	
	void Fire(const FVector& StartPosition, const FRotator& Direction, float Power, float DamageMultiplier) override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnCollision(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Expire();

	UFUNCTION(BlueprintCallable)
	void SetTeam(int TeamIndex);

	UFUNCTION(BlueprintCallable)
	int GetTeam()const;


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	void ApplyCorrection(const FVector& Direction) override;

protected:
	void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Replicated)
	FVector TargetLocation;

	FVector InitialDirection;
	FVector CurrentVelocity;
	FVector LastPosition;
	bool HasBeenFired;

	UPROPERTY()
	int TeamIndex = -1;

	float VInterpSpeed = 5;

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
