#pragma once
#include "CoreMinimal.h"
#include "NPHealthComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class NETWORKINGPROJECT_API UNPHealthComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UNPHealthComponent();


	UFUNCTION(BlueprintCallable)
	bool ReceiveDamage(int Amount);

	UFUNCTION(BlueprintCallable)
	void ReceiveHealing(int Amount);
	UFUNCTION(BlueprintCallable)
	int GetCurrentHealth();


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	int MaxHealth;

	UPROPERTY(Replicated)
	int Health;
};