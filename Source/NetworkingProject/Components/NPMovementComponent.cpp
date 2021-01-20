#include "NPMovementComponent.h"
#include "DrawDebugHelpers.h"

UNPMovementComponent::UNPMovementComponent()
{
	Gravity = 30;
	FacingRotationSpeed = 1;
	PrimaryComponentTick.bCanEverTick = true;
	bHasLaunchVelocity = false;
	bPendingXYOverride = false;
	bPendingZOverride = false;

	JumpHeight = 200.0f;
	JumpTime = 0.25f;
	JumpAccu = 0;
	JumpUpdateFreq = 0.01f;
}


void UNPMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FacingRotationCurrent = FQuat::Slerp(FacingRotationCurrent.Quaternion(), FacingRotationTarget.Quaternion(), FacingRotationSpeed * DeltaTime).Rotator();
	if (FacingRotationCurrent.Equals(FacingRotationTarget))
	{
		FacingRotationCurrent = FacingRotationTarget;
		SetComponentTickEnabled(false);
	}
}

FNPMovementData UNPMovementComponent::CreateMovementData() const
{
	FNPMovementData MovementData;
	MovementData.StartLocation = UpdatedComponent->GetComponentLocation();
	return MovementData;
}

void UNPMovementComponent::Move(FNPMovementData& FrameMovement)
{
	Hit.Reset();
	
	FVector Delta = GetMovementDelta(FrameMovement) + FVector(0, 0, AccumulatedGravity);

	if (bHasLaunchVelocity)
	{
		HandleLaunch(Delta);
	}
	
	MoveUpdatedComponent(Delta, FacingRotationCurrent, true, &Hit);

	auto OwnerLoc = GetOwner()->GetActorLocation();


	FVector GroundStartLocation = GetOwner()->GetActorLocation() - FVector(0, 0, CharacterHalfHeight);
	FVector GroundEndLocation = GroundStartLocation - FVector(0, 0, 10.0f);
	DrawDebugLine(GetWorld(), GroundStartLocation, GroundEndLocation, FColor::Yellow, false, 1.0f);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	CollisionQueryParams.bFindInitialOverlaps = true;

	FCollisionShape MyBox = FCollisionShape::MakeBox(FVector(44.0f, 44.0f, 10.0f));
	GetWorld()->SweepSingleByChannel(GroundHitResult, GroundStartLocation, GroundEndLocation, FQuat::Identity, ECC_WorldStatic, MyBox);

	DrawDebugBox(GetWorld(), GroundStartLocation, FVector(44.0f, 44.0f, 10.0f), FColor::Yellow, false, 1.0f);


	//GetWorld()->LineTraceSingleByChannel(GroundHitResult,
	//	GroundStartLocation,
	//	GroundEndLocation, ECC_WorldStatic, CollisionQueryParams);

	if ((Hit.bBlockingHit && FVector::DotProduct(FVector::UpVector, Hit.Normal) > 0.0f) || GroundHitResult.bBlockingHit && FVector::DotProduct(FVector::UpVector, GroundHitResult.Normal) > 0)
	{
		MovementState = EMS_Grounded;
		AccumulatedGravity = 0;
		Delta = GetMovementDelta(FrameMovement);
		GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Emerald, "Grounded");
	}
	else 
	{
		MovementState = EMS_Falling;
		GEngine->AddOnScreenDebugMessage(2, 0.05f, FColor::Emerald, "Not Grounded");
	}


	SlideAlongSurface(Delta, 1.0f - Hit.Time, Hit.Normal, Hit);

	FrameMovement.Hit = Hit;
	FrameMovement.FinalLocation = UpdatedComponent->GetComponentLocation();
}

void UNPMovementComponent::HandleLaunch(FVector& MovementDelta)
{
	FVector FinalVel = PendingLaunchVelocity;

	if (!bPendingXYOverride)
	{
		FinalVel.X += MovementDelta.X;
		FinalVel.Y += MovementDelta.Y;
	}
	if (!bPendingZOverride)
	{
		FinalVel.Z += MovementDelta.Z;
	}
	else
	{
		AccumulatedGravity = 0;
	}

	MovementDelta = FinalVel;
	bHasLaunchVelocity = false;
	bPendingXYOverride = false;
	bPendingZOverride = false;
}

void UNPMovementComponent::ApplyGravity()
{
	AccumulatedGravity -= Gravity * GetWorld()->GetDeltaSeconds();
}

void UNPMovementComponent::LaunchCharacter(const FVector& LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	PendingLaunchVelocity = LaunchVelocity;
	bPendingXYOverride = bXYOverride;
	bPendingZOverride = bZOverride;
	bHasLaunchVelocity = true;
}

void UNPMovementComponent::SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed)
{
	FRotator NewRotation = InFacingRotation;
	NewRotation.Roll = 0;
	NewRotation.Pitch = 0;
	FacingRotationTarget = NewRotation;

	if (InRotationSpeed < 0.0f)
	{
		FacingRotationCurrent = NewRotation;
		SetComponentTickEnabled(false);
	}
	else
	{
		SetComponentTickEnabled(true);
	}

}

void UNPMovementComponent::UpdateComponentRotationOnly()
{
	MoveUpdatedComponent(FVector::ZeroVector, FacingRotationCurrent, true, &Hit);
}

void UNPMovementComponent::Jump()
{
	if (!IsJumping() && CanJump())
	{
		GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, this, &UNPMovementComponent::Jump_Impl, JumpUpdateFreq, true);
	}
}

void UNPMovementComponent::Jump_Impl()
{
	auto DeltaTime = JumpUpdateFreq;
	JumpAccu += DeltaTime;
	if (JumpAccu >= JumpTime)
	{
		JumpAccu = 0;
		GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
	}
	else if (JumpTime != 0)
	{
		LaunchCharacter(FVector(0, 0, (JumpHeight / JumpTime) * DeltaTime), false, true);
	}
}

FVector UNPMovementComponent::GetMovementDelta(const FNPMovementData& FrameMovement) const
{
	return FrameMovement.MovementDelta;
}
