#include "NPPlayer.h"
#include "GameFramework/PlayerState.h"
#include "../Components/NPMovementComponent.h"
#include "Engine/CollisionProfile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Projectile/NPBaseProjectile.h"
#include "../Projectile/NPArrowProjectile.h"

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
		Mesh->bOwnerNoSee = true;
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

	FPMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh v"));
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
		FPMesh->SetupAttachment(CapsuleComponent);
		static FName MeshCollisionProfileName(TEXT("NoCollision"));
		FPMesh->SetCollisionProfileName(MeshCollisionProfileName);
		FPMesh->SetGenerateOverlapEvents(false);
		FPMesh->SetCanEverAffectNavigation(false);
	}
	FirePosition = CreateDefaultSubobject<USceneComponent>(TEXT("FirePosition v"));
	FirePosition->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ShootingComponent = CreateDefaultSubobject<UNPShootingComponent>(TEXT("ShootingComponent v"));

	AmmoComponent = CreateDefaultSubobject<UNPAmmoComponent>(TEXT("AmmoComponent v"));
	AmmoComponent->AddAmmoContainer(UNPArrowProjectile::StaticClass(), 30, 30);

	HealthComponent = CreateDefaultSubobject<UNPHealthComponent>(TEXT("HealthComponent v"));

	Projectiles.SetNum(10);
	for (int i = 0; i < Projectiles.Num(); i++)
	{
		Projectiles[i] = CreateDefaultSubobject<UNPArrowProjectile>(TEXT("Projectiles V " + i));
		Projectiles[i]->AddToRoot();

	}
	MovementSpeed = 500.0f;

	ArrowForce = 2000.0f;
	MovementInput = FVector::ZeroVector;
	TargetLocation = GetActorLocation();

}

void ANP_Player::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < Projectiles.Num(); i++)
	{
		Projectiles[i]->SetStaticMesh(ProjectileMesh);
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
	Multicast_FireProjectile(ProjectileToShoot, ArrowStartPosition, FacingRotation);
}

void ANP_Player::Multicast_FireProjectile_Implementation(int ProjectileToShoot, const FVector& ArrowStartPosition, const FRotator& FacingRotation)
{
	if (!ensure(ProjectileToShoot != -1))
		return;

	//float ChargeAmount = ShootingComponent->Fire();
	//ProjectileToShoot->Fire(this, GetControlRotation().Vector(), ChargeAmount * ArrowForce, ArrowDamageMultiplier);

	//if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		//NewRocket->ApplyCorrection(FacingRotation.Vector());
	}
	//else
	{
		float ChargeAmount = ShootingComponent->Fire();
		Projectiles[ProjectileToShoot]->Fire(ArrowStartPosition, FacingRotation, ChargeAmount * ArrowForce , ArrowDamageMultiplier);
		//((ANPBaseProjectile*)ProjectilePool->Pool[ProjectileToShoot])->Fire(this, ArrowStartPosition, FacingRotation, ChargeAmount * ArrowForce, ArrowDamageMultiplier);
	}
}

void ANP_Player::Server_StartCharging_Implementation()
{
	ShootingComponent->StartCharging();
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

void ANP_Player::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (int i = 0; i < Projectiles.Num(); i++)
	{
		Projectiles[i]->RemoveFromRoot();
	}
	Projectiles.Empty();
	Super::EndPlay(EndPlayReason);
}

void ANP_Player::JumpReleased_Implementation()
{

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
	if (counter >= 10)
	{
		counter = 0;
	}

	////if (ProjectilePool->HasFreeObject())
//{
////	ANPBaseProjectile* BaseProjectile = (ANPBaseProjectile*)ProjectilePool->GetPooledObject();
////	Server_FireProjectile(BaseProjectile->Index, GetActorLocation(), GetControlRotation());
//}
////else
//{
////	ShootingComponent->StopCharging();
//}
///*if (GetLocalRole() >= ROLE_AutonomousProxy)
//{*/
//	//if (HasAuthority())
//	//{
//		
//	//}
//	//else
//	//{
//	//	if (ProjectilePool->HasFreeObject())
//	//	{
//	//		ANPBaseProjectile* BaseProjectile = (ANPBaseProjectile*)ProjectilePool->GetPooledObject();
//	//		//start moving
//	//		//And then correct position later
//	//		Server_FireProjectile(BaseProjectile, FirePosition->GetComponentLocation(), GetControlRotation());
//	//	}
//	//	else
//	//	{
//	//		ShootingComponent->StopCharging();
//	//	}
//	//}
//	//
////}
}

void ANP_Player::FireButtonPressed_Implementation()
{
	Server_StartCharging();
	ShootingComponent->StartCharging();
}

void ANP_Player::JumpPressed_Implementation()
{
	if (MovementComponent->CanJump())
	{
		MovementComponent->Jump();
	}
}

bool ANP_Player::CanDamage_Implementation() const
{
	return true;
}

FDamageResult ANP_Player::ReceiveDamage_Implementation(float Damage, AActor* _Instigator)
{
	FDamageResult DamageResult = FDamageResult();
	//calculate mitigation etc.
	DamageResult.HealthDamageDealt = Damage;
	if (IDamageableInterface::Execute_CanDamage(this))
	{
		//calculate actual damage dealt

		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Cyan, "Ouch oof owwie!");
		if (!HealthComponent->ReceiveDamage(Damage))
		{
			//We died
		}
	}
	return DamageResult;
}


