#include "NPMovementComponent.h"

UNPMovementComponent::UNPMovementComponent()
{
	Gravity = 30;
	FacingRotationSpeed = 1;
	PrimaryComponentTick.bCanEverTick = true;

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
	FVector Delta = GetMovementDelta(FrameMovement);
	MoveUpdatedComponent(Delta, FacingRotationCurrent, true, &Hit);

	if (Hit.bBlockingHit && FVector::DotProduct(FVector::UpVector, Hit.Normal) > 0.0f)
	{
		AccumulatedGravity = 0;
		Delta = GetMovementDelta(FrameMovement);
	}

	SlideAlongSurface(Delta, 1.0f - Hit.Time, Hit.Normal, Hit);

	FrameMovement.Hit = Hit;
	FrameMovement.FinalLocation = UpdatedComponent->GetComponentLocation();
}

void UNPMovementComponent::ApplyGravity()
{
	AccumulatedGravity -= Gravity * GetWorld()->GetDeltaSeconds();
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

FVector UNPMovementComponent::GetMovementDelta(const FNPMovementData& FrameMovement) const
{
	return FrameMovement.MovementDelta;
}
