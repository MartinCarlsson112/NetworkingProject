#pragma once
#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "../NPMovementData.h"
#include "NPMovementComponent.generated.h"

UCLASS()
class NETWORKINGPROJECT_API UNPMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
public:
	UNPMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FNPMovementData CreateMovementData() const;

	void Move(FNPMovementData& FrameMovement);
	void ApplyGravity();

	UPROPERTY(EditAnywhere, Category=Movement)
	float Gravity;

	UPROPERTY(EditAnywhere, Category = Movement)
	float FacingRotationSpeed;

	FVector GetGravityAsVector() const { return FVector(0, 0, AccumulatedGravity); }
	FRotator GetFacingRotation() const { return FacingRotationCurrent; }
	FVector GetFacingDirection() const { return FacingRotationCurrent.Vector(); }


private:

	void SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed = -1);
	FVector GetMovementDelta(const FNPMovementData& FrameMovement) const;

	FHitResult Hit;
	FRotator FacingRotationCurrent;
	FRotator FacingRotationTarget;
	float AccumulatedGravity;
};