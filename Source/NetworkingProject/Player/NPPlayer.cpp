#include "NPPlayer.h"
#include "GameFramework/PlayerState.h"
#include "../Components/NPMovementComponent.h"
#include "Engine/CollisionProfile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Projectile/NPArrowProjectile.h"

ANP_Player::ANP_Player()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicateMovement(false);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;


	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	if (Mesh)
	{
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = true;
		Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Mesh->SetupAttachment(RootComponent);
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCanEverAffectNavigation(false);
	}

	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FPMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	if (FPMesh)
	{
		FPMesh->AlwaysLoadOnClient = true;
		FPMesh->AlwaysLoadOnServer = true;
		FPMesh->bOwnerNoSee = false;
		FPMesh->bOnlyOwnerSee = true;
		FPMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		FPMesh->bCastDynamicShadow = true;
		FPMesh->bAffectDynamicIndirectLighting = true;
		FPMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		FPMesh->SetupAttachment(FPSCamera);
		static FName MeshCollisionProfileName(TEXT("NoCollision"));
		FPMesh->SetCollisionProfileName(MeshCollisionProfileName);
		FPMesh->SetGenerateOverlapEvents(false);
		FPMesh->SetCanEverAffectNavigation(false);
	}
	FirePosition = CreateDefaultSubobject<USceneComponent>(TEXT("FirePosition"));
	FirePosition->AttachToComponent(FPMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonSpringArm"));
	ThirdPersonSpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->AttachToComponent(ThirdPersonSpringArm, FAttachmentTransformRules::KeepRelativeTransform);

	ShootingComponent = CreateDefaultSubobject<UNPShootingComponent>(TEXT("ShootingComponent"));
	AmmoComponent = CreateDefaultSubobject<UNPAmmoComponent>(TEXT("AmmoComponent"));
	HealthComponent = CreateDefaultSubobject<UNPHealthComponent>(TEXT("HealthComponent"));
	MovementComponent = CreateDefaultSubobject<UNPMovementComponent>(TEXT("MovementComponent"));

	const FName ProjectileNames[] = {
		TEXT("Projectile_1"),
		TEXT("Projectile_2"),
		TEXT("Projectile_3"),
		TEXT("Projectile_4"),
		TEXT("Projectile_5"),
		TEXT("Projectile_6"),
		TEXT("Projectile_7"),
		TEXT("Projectile_8"),
		TEXT("Projectile_9"),
		TEXT("Projectile_0"),
	};
	Arrows.Empty();
	for (int i = 0; i < 10; i++)
	{
		auto Arrow = CreateDefaultSubobject<UNPArrowProjectile>(ProjectileNames[i]);
		Arrow->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Arrow->SetIsReplicated(true);
		Arrows.Add(Arrow);
	}

	MovementSpeed = 500.0f;
	ArrowForce = 2000.0f;
	NumberOfArrowInstances = 10;
	MovementInput = FVector::ZeroVector;
	TargetLocation = GetActorLocation();
}

void ANP_Player::BeginPlay()
{
	Super::BeginPlay();		
	ensure(ProjectileMesh != nullptr);
	for (int i = 0; i < Arrows.Num(); i++)
	{
		Arrows[i]->SetStaticMesh(ProjectileMesh);
	}
}

void ANP_Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsLocallyControlled())
	{
		FNPMovementData MovementData = MovementComponent->CreateMovementData();
		MovementComponent->SetFacingRotation(GetActorRotation());
		MovementComponent->ApplyGravity();

		auto Forward = GetActorForwardVector();
		auto Right = GetActorRightVector();
		MovementData.MovementDelta = (Forward * MovementInput.X + Right * MovementInput.Y) * DeltaSeconds * MovementSpeed;
		MovementComponent->Move(MovementData);

		Server_SendMove(GetActorLocation());
		Server_SendRotation(GetActorRotation());
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
		if (!ReplicatedRotation.Equals(GetActorRotation()))
		{
			const FRotator NewRot = FMath::RInterpTo(GetActorRotation(), ReplicatedRotation, DeltaSeconds, RotationInterpolationSpeed);
			SetActorRotation(NewRot);
		}
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
	ReplicatedRotation = NewRotation;
}

void ANP_Player::Server_FireProjectile_Implementation(int ProjectileToShoot, const FVector& ArrowStartPosition, const FRotator& FacingRotation)
{
	if (AmmoComponent->UseAmmo(SelectedAmmoType))
	{
		Multicast_FireProjectile(ProjectileToShoot, ArrowStartPosition, FacingRotation);
	}
}

void ANP_Player::Multicast_FireProjectile_Implementation(int ProjectileToShoot, const FVector& ArrowStartPosition, const FRotator& FacingRotation)
{
	if (!ensure(ProjectileToShoot != -1))
		return;

	//if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		//NewRocket->ApplyCorrection(FacingRotation.Vector());
	}
	//else
	{
		float ChargeAmount = ShootingComponent->Fire();
		Arrows[ProjectileToShoot]->Fire(ArrowStartPosition, FacingRotation, ChargeAmount * ArrowForce , ArrowDamageMultiplier);
	}
}

void ANP_Player::Server_StartCharging_Implementation()
{
	if (AmmoComponent->GetAmmoCount(SelectedAmmoType) > 0)
	{
		ShootingComponent->StartCharging();
	}
}

int32 ANP_Player::GetPing() const
{
	auto State = GetPlayerState();
	if (State)
	{
		return static_cast<int32>(State->GetPing()) * 4;
	}
	return 0;
}


void ANP_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANP_Player, ReplicatedRotation);
}

void ANP_Player::ReceivePickup_Implementation(const FNPAmmoPickupData& AmmoData)
{
	AmmoComponent->GainAmmo(AmmoData.AmmoType, AmmoData.Count);
}

bool ANP_Player::CanPickup_Implementation(const FNPAmmoPickupData& AmmoData)
{
	if (AmmoComponent->CanGainAmmo(AmmoData.AmmoType))
	{
		return true;
	}
	return false;
}


void ANP_Player::Look_Implementation(float Value)
{
	AddControllerPitchInput(Value);
}

void ANP_Player::Turn_Implementation(float Value)
{
	AddControllerYawInput(Value);
}

void ANP_Player::MoveRight_Implementation(float value)
{
	MovementInput.Y = value;
}

void ANP_Player::MoveForward_Implementation(float value)
{
	MovementInput.X = value;
}

void ANP_Player::FireButtonReleased_Implementation()
{
	auto FacingRotation = GetControlRotation();
	Server_FireProjectile(counter, FirePosition->GetComponentLocation(), FacingRotation);
	counter++;
	if (counter >= Arrows.Num())
	{
		counter = 0;
	}
}

void ANP_Player::FireButtonPressed_Implementation()
{
	Server_StartCharging();

	if (AmmoComponent->GetAmmoCount(SelectedAmmoType) > 0)
	{
		ShootingComponent->StartCharging();
	}
}

void ANP_Player::JumpPressed_Implementation()
{
	if (MovementComponent->CanJump())
	{
		MovementComponent->Jump();
	}
}

void ANP_Player::JumpReleased_Implementation()
{

}

bool ANP_Player::CanDamage_Implementation() const
{
	//For example if target is not supposed to be hittable etc.
	return true;
}

FDamageResult ANP_Player::ReceiveDamage_Implementation(float Damage, AActor* _Instigator)
{
	FDamageResult DamageResult = FDamageResult();
	//calculate mitigation etc.
	DamageResult.HealthDamageDealt = Damage;
	if (!HealthComponent->ReceiveDamage(Damage))
	{
		//We died
	}
	return DamageResult;
}


