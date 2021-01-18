

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NPPlayerController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class NETWORKINGPROJECT_API ANPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void StartSession();

	UFUNCTION(BlueprintCallable)
	void EndSession();

	UFUNCTION()	
	void JumpPressed();

	UFUNCTION()	
	void JumpReleased();

	UFUNCTION()	
	void FireButtonPressed();

	UFUNCTION()
	void FireButtonReleased();

	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void Turn(float value);

	UFUNCTION()
	void Look(float value);

protected:



	void SetupInputComponent() override;

};
