#pragma once
#include "DamageableInterface.generated.h"

class ANP_Player;

USTRUCT(BlueprintType)
struct FDamageResult
{
	GENERATED_BODY()

	float HealthDamageDealt = 0;
	float AbsorbDamageDealt = 0;
	int32 flags = 0;
};

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
	FDamageResult ReceiveDamage(float Damage, AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category =Damage)
	bool CanDamage() const;
};

