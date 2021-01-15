#include "NPArrowProjectile.h"

UNPArrowProjectile::UNPArrowProjectile()
{
	PrimaryComponentTick.bCanEverTick = true;
	ArrowLifetime = 15.0f;
	ArrowHitLifetime = 10.0f;
	SetIsReplicatedByDefault(true);
	SetComponentTickEnabled(false);
	SetNotifyRigidBodyCollision(false);
}

void UNPArrowProjectile::Fire(AActor* Shooter, const FVector& StartPosition, const FRotator& Direction, float Power, float DamageMultiplier)
{
	SetWorldLocation(StartPosition);
	InitialDirection = Direction.Vector();
	SetWorldRotation(Direction);
	CurrentVelocity = Direction.Vector() * Power;

	SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetComponentTickEnabled(true);
	SetGenerateOverlapEvents(true);
	SetHiddenInGame(false);

	GetWorld()->GetTimerManager().SetTimer(LifetimeTimerHandle, this, &UNPArrowProjectile::Expire, ArrowLifetime, false);
}


void UNPArrowProjectile::OnCollision(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner() == OtherActor)
	{
		return;
	}

	//Attack target
	//Other->ImplementsInterface
	//if Other->CanDamage()
		//Other->ReceiveDamage(CurrentDamage)

	//Disable Collisions
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Disable movement
	SetComponentTickEnabled(false);

	//Start timer to return the object
	if (GetWorld()->GetTimerManager().IsTimerActive(LifetimeTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(LifetimeTimerHandle);
	}
	GetWorld()->GetTimerManager().SetTimer(LifetimeTimerHandle, this, &UNPArrowProjectile::Expire, ArrowHitLifetime, false);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.1f , FColor::Blue, "Collided");
}

void UNPArrowProjectile::Expire()
{
	SetHiddenInGame(true);
	//disable collision
	SetComponentTickEnabled(false);
}

void UNPArrowProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	SetSimulatePhysics(false);
	SetHiddenInGame(true);

	SetNotifyRigidBodyCollision(false);
	SetGenerateOverlapEvents(true);
	SetCollisionProfileName("OverlapAll");

	SetGenerateOverlapEvents(true);
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
