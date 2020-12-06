#include "NPPlayer.h"
#include "GameFramework/PlayerState.h"
#include "../Components/NPMovementComponent.h"
#include "Engine/CollisionProfile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

ANP_Player::ANP_Player()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicateMovement(false);

	MovementComponent = CreateDefaultSubobject<UNPMovementComponent>(TEXT("MovementComponent v"));

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent v"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent v"));
	if (Mesh)
	{
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = false;
		Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Mesh->SetupAttachment(CapsuleComponent);
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCanEverAffectNavigation(false);
	}

}

void ANP_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ANP_Player::OnJumpPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ANP_Player::OnJumpReleased);

	PlayerInputComponent->BindAxis("Forward", this, &ANP_Player::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ANP_Player::MoveRight);


	PlayerInputComponent->BindAxis("Turn", this, &ANP_Player::Turn);
	//PlayerInputComponent->BindAxis("TurnRate", this, &ANP_Player::TurnAtRate);
	PlayerInputComponent->BindAxis("Look", this, &ANP_Player::Look);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &ANP_Player::LookUpAtRate);

}

void ANP_Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FNPMovementData MovementData = MovementComponent->CreateMovementData();

	if (IsLocallyControlled())
	{
		MovementComponent->SetFacingRotation(GetActorRotation());
		Server_SendRotation(MovementComponent->GetFacingRotation());

		MovementComponent->ApplyGravity();
		MovementData.MovementDelta = (GetActorForwardVector() * MovementInput.X + GetActorRightVector() * MovementInput.Y) * DeltaSeconds * 100.0f;
		MovementComponent->Move(MovementData);

		Server_SendMove(GetActorLocation());
	}

	else
	{

		if (!TargetLocation.Equals(GetActorLocation()))
		{
			const FVector NewLoc = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaSeconds, LocationInterpolationSpeed);

			SetActorLocation(NewLoc);

			float distance = FVector::Dist(GetActorLocation(), TargetLocation);
			if (distance < MinDistance)
			{
				SetActorLocation(TargetLocation);
			}
		}
		const FRotator NewRot = FMath::RInterpTo(MovementComponent->GetFacingRotation(), TargetRotation, DeltaSeconds, RotationInterpolationSpeed);
		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Blue, NewRot.ToString());
		MovementComponent->SetFacingRotation(NewRot);
		SetActorRotation(NewRot);
		MovementComponent->Move(MovementData);
	}

}

void ANP_Player::Server_SendMove_Implementation(const FVector& NewLocation)
{
	Multicast_SendLocation(NewLocation);
}


void ANP_Player::Multicast_SendLocation_Implementation(const FVector& NewLocation)
{
	if (!IsLocallyControlled())
	{
		TargetLocation = NewLocation;
	}
}

void ANP_Player::Server_SendRotation_Implementation(const FRotator& NewRotation)
{
	Multicast_SendRotation(NewRotation);
}

void ANP_Player::Multicast_SendRotation_Implementation(const FRotator& NewRotation)
{
	if (!IsLocallyControlled())
	{

		
		TargetRotation = NewRotation;
	}
}

int32 ANP_Player::GetPing() const
{
	auto State = GetPlayerState();

	if (State)
	{
		return static_cast<int32>(State->GetPing());
	}

	return 0;
}

void ANP_Player::OnJumpPressed()
{

}

void ANP_Player::OnJumpReleased()
{

}

void ANP_Player::MoveForward(float value)
{
	MovementInput.X = value;
}

void ANP_Player::MoveRight(float value)
{
	MovementInput.Y = value;
}

void ANP_Player::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ANP_Player::Look(float Value)
{
	AddControllerPitchInput(Value);
}

void ANP_Player::BeginPlay()
{
	Super::BeginPlay();

}

