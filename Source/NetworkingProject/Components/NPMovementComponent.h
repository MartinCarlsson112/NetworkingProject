#pragma once
#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../NPMovementData.h"
#include "NPMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMovementStateDelegate);

UENUM(BlueprintType)
enum EMovementState
{
	EMS_Idle,
	EMS_Running,
	EMS_Climbing,
	EMS_Dashing,
	EMS_Grounded,
	EMS_Jumping,
	EMS_Falling,
	EMS_Disabled
};

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class NETWORKINGPROJECT_API UNPMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
public:
	UNPMovementComponent();

	void UpdateMovement(float DeltaTime);

	FVector GetInput() const;
	void SetInput(float XAxis, float YAxis);
	void LaunchCharacter(const FVector& LaunchVelocity, bool bXYOverride, bool bZOverride);

	void HandleLaunch(FVector& MovementDelta);

	bool CanJump() const;

	bool Jumping;

	FVector InputVector;
	FVector Velocity;
	FVector Gravity = FVector(0, 0, -20.f);
	FVector GravityVector;
	TArray<struct FHitResult> CollisionResults; 
	TArray<struct FHitResult> GroundCheckResults;
	float Friction = 1.0f;
	float Acceleration = 500.0f;

	TEnumAsByte<EMovementState> MovementState;
	FVector Forward = FVector(1, 0, 0);
	FVector Right = FVector(0, 1, 0);

	FVector LaunchCharacterForce;


	bool CheckGrounded(FVector Normal);

	FTimerHandle JumpTimerHandle;
	bool IsJumping() const {
		return GetWorld()->GetTimerManager().IsTimerActive(JumpTimerHandle);
	}
	void Jump();
	void Jump_Impl();
	void StopJump();


	void BeginPlay() override;
	FCollisionShape CapsuleShape;
	UCapsuleComponent* CapsuleComponent;

	FVector PendingLaunchVelocity;
	bool bHasLaunchVelocity;
	bool bPendingZOverride;
	bool bPendingXYOverride;
	float JumpAccu;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Jumping)
		float JumpUpdateFreq;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Jumping)
		float JumpHeight;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Jumping)
		float JumpTime;

};