#pragma once
#include "CoreMinimal.h"
#include "NPHealthComponent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class NETWORKINGPROJECT_API UNPHealthComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UNPHealthComponent();


	UFUNCTION(BlueprintCallable)
	bool ReceiveDamage(float Amount);

	UFUNCTION(BlueprintCallable)
	void ReceiveHealing(float Amount);

private:

	float MaxHealth;
	float Health;
};