#include "NPArrowProjectile.h"
#include "../Interfaces/DamageableInterface.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
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
	InitialDirection = Direction.Vector();
	CurrentVelocity = Direction.Vector() * Power;
	ActivateProjectile();

	SetWorldRotation(Direction);
	SetWorldLocation(StartPosition);
	LastPosition = StartPosition;
	TargetLocation = StartPosition + CurrentVelocity;
	
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		GetWorld()->GetTimerManager().SetTimer(LifetimeTimerHandle, this, &UNPArrowProjectile::Expire, ArrowLifetime, false);
	}


}


void UNPArrowProjectile::OnCollision(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner() == OtherActor)
	{
		return;
	}		
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UDamageableInterface::StaticClass()))
		{
			if (IDamageableInterface::Execute_CanDamage(OtherActor))
			{
				IDamageableInterface::Execute_ReceiveDamage(OtherActor, ArrowDamage, GetOwner());
			}
		}
		//predict health change on client
		CurrentVelocity = FVector::ZeroVector;
		//Disable Collisions
		SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Disable movement
		SetComponentTickEnabled(false);
		SetActiveFlag(false);

		//Start timer for arrow to expire after hitting a target
		if (GetWorld()->GetTimerManager().IsTimerActive(LifetimeTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(LifetimeTimerHandle);
		}
		GetWorld()->GetTimerManager().SetTimer(LifetimeTimerHandle, this, &UNPArrowProjectile::Expire, ArrowHitLifetime, false);
	}
}

void UNPArrowProjectile::Expire()
{
	DeactivateProjectile();
}

void UNPArrowProjectile::BeginPlay()
{
	Super::BeginPlay();
	DeactivateProjectile();
	SetNotifyRigidBodyCollision(false);
	SetCollisionProfileName("OverlapAll");	
	OnComponentBeginOverlap.AddDynamic(this, &UNPArrowProjectile::OnCollision);
}

void UNPArrowProjectile::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		auto DeltaVelocity = DeltaTime * CurrentVelocity;
		auto DeltaGravity = (FVector(0, 0, -9.81f));

		auto MovementVector = DeltaVelocity + DeltaGravity;


		//DrawDebugLine(GetWorld(), GetComponentLocation(), GetComponentLocation() + DeltaVelocity + DeltaGravity, FColor::Red, false, 1.0f);
		SetWorldLocation(GetComponentLocation() + DeltaVelocity + DeltaGravity);

		CurrentVelocity = CurrentVelocity + DeltaGravity;

		SetWorldRotation(MovementVector.ToOrientationRotator());

		TargetLocation = GetComponentLocation();
	}
	else
	{
		FVector InterpolatedPosition = FMath::VInterpTo(LastPosition, TargetLocation, DeltaTime, VInterpSpeed);
		LastPosition = InterpolatedPosition;
		SetWorldLocation(InterpolatedPosition);
	}
}

void UNPArrowProjectile::SetTeam(int _TeamIndex)
{
	TeamIndex = _TeamIndex;
}

int UNPArrowProjectile::GetTeam() const
{
	return TeamIndex;
}

void UNPArrowProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UNPArrowProjectile, TargetLocation);
}

void UNPArrowProjectile::ApplyCorrection(const FVector& Direction)
{
	//TODO: implement this
}
