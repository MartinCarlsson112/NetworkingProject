

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerControlInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerControlInterface : public UInterface
{
	GENERATED_BODY()
};

class NETWORKINGPROJECT_API IPlayerControlInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	void JumpPressed();


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	void JumpReleased();


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	void FireButtonPressed();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	void FireButtonReleased();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	void MoveForward(float value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	void MoveRight(float value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	void Turn(float value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Input)
	void Look(float value);
};
