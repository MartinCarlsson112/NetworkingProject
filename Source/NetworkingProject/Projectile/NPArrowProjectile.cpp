#include "NPArrowProjectile.h"
#include "../Interfaces/DamageableInterface.h"
UNPArrowProjectile::UNPArrowProjectile()
{
	PrimaryComponentTick.bCanEverTick = true;
	ArrowLifetime = 15.0f;
	ArrowHitLifetime = 10.0f;
	SetIsReplicatedByDefault(true);
	SetComponentTickEnabled(false);
	SetNotifyRigidBodyCollision(false);
}


void UNPArrowProjectile::Fire(const FVector& StartPosition, const FRotator& Direction, float Power, float DamageMultiplier)
{
	SetWorldLocation(StartPosition);
	InitialDirection = Direction.Vector();
	SetWorldRotation(Direction);
	CurrentVelocity = Direction.Vector() * Power;
	ActivateArrow();
	GetWorld()->GetTimerManager().SetTimer(LifetimeTimerHandle, this, &UNPArrowProjectile::Expire, ArrowLifetime, false);
}


void UNPArrowProjectile::OnCollision(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner() == OtherActor)
	{
		return;
	}

	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.1f, FColor::Blue, "Collided");

		if (OtherActor->GetClass()->ImplementsInterface(UDamageableInterface::StaticClass()))
		{
			IDamageableInterface::Execute_ReceiveDamage(OtherActor, ArrowDamage, GetOwner());
		}
	}
	//predict health change

	//Disable Collisions
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Disable movement
	SetComponentTickEnabled(false);

	//Start timer for arrow to expire after hitting a target
	if (GetWorld()->GetTimerManager().IsTimerActive(LifetimeTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(LifetimeTimerHandle);
	}
	GetWorld()->GetTimerManager().SetTimer(LifetimeTimerHandle, this, &UNPArrowProjectile::Expire, ArrowHitLifetime, false);
}

void UNPArrowProjectile::Expire()
{
	DeactivateArrow();
}

void UNPArrowProjectile::BeginPlay()
{
	Super::BeginPlay();
	DeactivateArrow();

	SetNotifyRigidBodyCollision(false);
	SetCollisionProfileName("OverlapAll");
	OnComponentBeginOverlap.AddDynamic(this, &UNPArrowProjectile::OnCollision);
}


//On collision
//set ActorTickEnabled(false)
//
//return the pooled object

void UNPArrowProjectile::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto DeltaVelocity = DeltaTime * CurrentVelocity;
	auto DeltaGravity = (FVector(0, 0, -9.81f));

	auto MovementVector = DeltaVelocity + DeltaGravity;
	SetWorldLocation(GetComponentLocation() + DeltaVelocity + DeltaGravity);
	CurrentVelocity = CurrentVelocity + DeltaGravity;

	SetWorldRotation(MovementVector.ToOrientationRotator());

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, DeltaTime, FColor::Blue, GetComponentLocation().ToString());
}

void UNPArrowProjectile::SetTeam(int _TeamIndex)
{
	TeamIndex = _TeamIndex;
}

int UNPArrowProjectile::GetTeam() const
{
	return TeamIndex;
}
