#pragma once
#include "DamageableInterface.generated.h"

class ANP_Player;

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

class IDamageableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Damage)
	void ReceiveDamage(float Damage, ANP_Player* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category =Damage)
	bool CanDamage() const;
};

