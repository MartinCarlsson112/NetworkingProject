#include "NPAmmoPickup.h"
#include "../Interfaces/PickupInterface.h"

UNPAmmoPickup::UNPAmmoPickup()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	SetComponentTickEnabled(false);
	SetNotifyRigidBodyCollision(false);
	SetCollisionProfileName("OverlapAll");
	ActivatePickup();
}

void UNPAmmoPickup::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
		{
			if (IPickupInterface::Execute_CanPickup(OtherActor, AmmoData))
			{
				IPickupInterface::Execute_ReceivePickup(OtherActor, AmmoData);
			}
			OnPickup.Broadcast();
			DeactivatePickup();
		}
	}
}


void UNPAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UNPAmmoPickup::OnOverlapBegin);
}

void UNPAmmoPickup::ActivatePickup()
{
	SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetGenerateOverlapEvents(true);
}

void UNPAmmoPickup::DeactivatePickup()
{
	SetGenerateOverlapEvents(false);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
