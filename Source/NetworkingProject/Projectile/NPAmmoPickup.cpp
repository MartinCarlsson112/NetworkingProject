#include "NPAmmoPickup.h"
#include "../Interfaces/PickupInterface.h"
#include "Net/UnrealNetwork.h"
UNPAmmoPickup::UNPAmmoPickup()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	SetComponentTickEnabled(false);
	SetNotifyRigidBodyCollision(false);
	SetCollisionProfileName("OverlapAll");
	ActivatePickup();
	bUsed = false;
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
				OnPickup.Broadcast();
				DeactivatePickup();
				bUsed = true;
			}
		}
	}
	else if(!bUsed)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
		{
			if (IPickupInterface::Execute_CanPickup(OtherActor, AmmoData))
			{
				IPickupInterface::Execute_ReceivePickup(OtherActor, AmmoData);
				SetVisibility(false, true);
				bUsed = true;
			}
		}
	}
}


void UNPAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UNPAmmoPickup::OnOverlapBegin);
}

void UNPAmmoPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UNPAmmoPickup, bUsed);
}

void UNPAmmoPickup::ActivatePickup()
{
	SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetGenerateOverlapEvents(true);
	SetVisibility(true, true);
}

void UNPAmmoPickup::DeactivatePickup()
{
	SetGenerateOverlapEvents(false);
	SetVisibility(false, true);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
