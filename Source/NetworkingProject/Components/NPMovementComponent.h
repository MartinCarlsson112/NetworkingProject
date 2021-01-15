#pragma once
#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "../NPMovementData.h"
#include "NPMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMovementStateDelegate);

UENUM(BlueprintType)
enum EMovementState
{
	EMS_Grounded,
	EMS_InAir,
	EMS_Disabled
};

UCLASS(Blueprintable, BlueprintType)
class NETWORKINGPROJECT_API UNPMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
public:
	UNPMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FNPMovementData CreateMovementData() const;

	void Move(FNPMovementData& FrameMovement);

	void HandleLaunch(FVector& MovementDelta);
	void ApplyGravity();
	void LaunchCharacter(const FVector& LaunchVelocity, bool bXYOverride, bool bZOverride);

	UPROPERTY(EditAnywhere, Category=Movement)
	float Gravity;

	UPROPERTY(EditAnywhere, Category = Movement)
	float FacingRotationSpeed;

	FVector GetGravityAsVector() const { return FVector(0, 0, AccumulatedGravity); }
	FRotator GetFacingRotation() const { return FacingRotationCurrent; }
	FVector GetFacingDirection() const { return FacingRotationCurrent.Vector(); }

	void SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed = -1);

	void UpdateComponentRotationOnly();

	FTimerHandle JumpTimerHandle;
	bool CanJump() const {
		return MovementState == EMS_Grounded;
	}
	bool IsJumping() const{
		return GetWorld()->GetTimerManager().IsTimerActive(JumpTimerHandle);
	}
	void Jump();
	void Jump_Impl();



	UPROPERTY(BlueprintAssignable)
	FMovementStateDelegate Jumped;

	UPROPERTY(BlueprintAssignable)
	FMovementStateDelegate DoubleJumped;

	UPROPERTY(BlueprintAssignable)
	FMovementStateDelegate ClimbJumped;

	UPROPERTY(BlueprintAssignable)
	FMovementStateDelegate WallrunJumped;

	UPROPERTY(BlueprintAssignable)
	FMovementStateDelegate Dashed;

	UPROPERTY(BlueprintAssignable)
	FMovementStateDelegate StartedWallrunning;

	UPROPERTY(BlueprintAssignable)
	FMovementStateDelegate StoppedWallrunning;

	UPROPERTY(BlueprintAssignable)
	FMovementStateDelegate StartedClimbing;

	UPROPERTY(BlueprintAssignable)
	FMovementStateDelegate StoppedClimbing;

private:
	FVector GetMovementDelta(const FNPMovementData& FrameMovement) const;

	FVector PendingLaunchVelocity;
	bool bHasLaunchVelocity;
	bool bPendingZOverride;
	bool bPendingXYOverride;

	FHitResult GroundHitResult;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = General)
	float CharacterHalfHeight;

	float JumpAccu;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Jumping)
	float JumpUpdateFreq;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Jumping)
	float JumpHeight;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Jumping)
	float JumpTime;

	EMovementState MovementState;
	FHitResult Hit;
	FRotator FacingRotationCurrent;
	FRotator FacingRotationTarget;
	float AccumulatedGravity;
};