#pragma once

#include "CoreMinimal.h"
#include "NPProjectileMovementComponent.generated.h"


UCLASS(Blueprintable, BlueprintType)
class NETWORKINGPROJECT_API UNPProjectileMovementComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UNPProjectileMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Move();

	void ApplyGravity();
	
	UPROPERTY(EditAnywhere, Category = Movement)
	float Gravity;

	UPROPERTY(EditAnywhere, Category = Movement)
	float FacingRotationSpeed;

	FVector GetGravityAsVector() const { return FVector(0, 0, AccumulatedGravity); }
	FRotator GetFacingRotation() const { return FacingRotationCurrent; }
	FVector GetFacingDirection() const { return FacingRotationCurrent.Vector(); }

	void SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed = -1);

	void UpdateComponentRotationOnly();


private:
	FHitResult Hit;
	FRotator FacingRotationCurrent;
	FRotator FacingRotationTarget;
	float AccumulatedGravity;


};