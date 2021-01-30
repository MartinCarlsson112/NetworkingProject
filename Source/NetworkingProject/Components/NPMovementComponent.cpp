#include "NPMovementComponent.h"

#include "DrawDebugHelpers.h"
UNPMovementComponent::UNPMovementComponent()
{
	Jumping = false;

	PrimaryComponentTick.bCanEverTick = true;
	bHasLaunchVelocity = false;
	bPendingXYOverride = false;
	bPendingZOverride = false;

	JumpHeight = 300.0f;
	JumpTime = 0.35f;
	JumpAccu = 0;
	JumpUpdateFreq = 0.01f;
	MovementState = EMS_Falling;
}


void UNPMovementComponent::UpdateMovement(float DeltaTime)
{
	Velocity = GetInput() * Acceleration * DeltaTime;
	if (bHasLaunchVelocity)
	{
		HandleLaunch(Velocity);
	}
	Velocity += GravityVector * DeltaTime;
	FHitResult MoveHit;
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.1f, FColor::Blue, Velocity.ToString());
	MoveUpdatedComponent(Velocity, GetOwner()->GetActorRotation().Quaternion(), true, &MoveHit);
	//if(GetWorld()->LineTraceMultiByChannel(GroundCheckResults, UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + ))

	SlideAlongSurface(Velocity, 1.0f - MoveHit.Time, MoveHit.Normal, MoveHit);

	bool FoundGround = false;
	FHitResult GroundCheck;

	FVector MoveDirection = Velocity;

	MoveDirection.Normalize(0.001f);


	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	if (GetWorld()->SweepSingleByChannel(
		GroundCheck,
		UpdatedComponent->GetComponentLocation() + MoveDirection * 5.0f,
		UpdatedComponent->GetComponentLocation() + -UpdatedComponent->GetUpVector() * 5.0f, 
		FQuat::Identity, 
		ECC_WorldDynamic, 
		CapsuleShape, QueryParams))
	{
		if (CheckGrounded(GroundCheck.ImpactNormal))
		{
			Forward = FVector::CrossProduct(GroundCheck.ImpactNormal, -UpdatedComponent->GetRightVector());
			Right = FVector::CrossProduct(Forward, GroundCheck.ImpactNormal);
			//DrawDebugLine(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + Forward * 100.0f, FColor::Red, false, 1.0f);
			//DrawDebugLine(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + Right * 100.0f, FColor::Blue, false, 1.0f);
			//DrawDebugLine(GetWorld(), GroundCheck.ImpactPoint, GroundCheck.ImpactPoint + GroundCheck.ImpactNormal * 100.0f, FColor::Green, false, 1.0f);

			GravityVector = FVector::ZeroVector;
			FoundGround = true;
		}
		
	}

	if (!FoundGround)
	{
		GravityVector = Gravity;
		Forward = GetOwner()->GetActorForwardVector();
		MovementState = EMS_Falling;
		GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Blue, "Falling");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Blue, "Grounded");
		MovementState = EMS_Grounded;
	}

	//figure out if the player wants to wall climb
	if (Jumping)
	{
		FHitResult WallClimbTraceHit;
		auto WallClimbTraceStartLoc = UpdatedComponent->GetComponentLocation() + UpdatedComponent->GetForwardVector() * CapsuleComponent->GetScaledCapsuleRadius();
		if (GetWorld()->LineTraceSingleByChannel(
		WallClimbTraceHit, 
		WallClimbTraceStartLoc,
		WallClimbTraceStartLoc + UpdatedComponent->GetForwardVector() * 100.0f,
		ECC_WorldDynamic, 
		QueryParams))
		{
			LaunchCharacter(FVector(0, 0, 10), false, true);
		}
	}
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
		GravityVector = FVector::ZeroVector;
	}

	MovementDelta = FinalVel;
	bHasLaunchVelocity = false;
	bPendingXYOverride = false;
	bPendingZOverride = false;
}

void UNPMovementComponent::LaunchCharacter(const FVector& LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	PendingLaunchVelocity = LaunchVelocity;
	bPendingXYOverride = bXYOverride;
	bPendingZOverride = bZOverride;
	bHasLaunchVelocity = true;
}

FVector UNPMovementComponent::GetInput() const
{
	return InputVector;
}

void UNPMovementComponent::SetInput(float XAxis, float YAxis)
{
	InputVector = (Forward * XAxis) + (-Right* YAxis);
	InputVector.Normalize(0.001f);
}

void UNPMovementComponent::StopJump()
{
	Jumping = false;
}

bool UNPMovementComponent::CanJump() const
{
	return MovementState == EMS_Grounded;
}

bool UNPMovementComponent::CheckGrounded(FVector Normal)
{
	return FVector::DotProduct(FVector::UpVector, Normal) > 0.1f;
}

void UNPMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	CapsuleComponent = (UCapsuleComponent*)GetOwner()->GetComponentByClass(UCapsuleComponent::StaticClass());
	CapsuleShape = FCollisionShape::MakeCapsule(CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleHalfHeight());
}

void UNPMovementComponent::Jump()
{
	Jumping = true;
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
		LaunchCharacter(Forward + FVector(0, 0, (JumpHeight / JumpTime) * DeltaTime), false, true);
	}
}