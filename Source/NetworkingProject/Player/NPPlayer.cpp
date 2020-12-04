#include "NPPlayer.h"

void ANP_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);



}

void ANP_Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	throw std::logic_error("The method or operation is not implemented.");
}

void ANP_Player::BeginPlay()
{
	Super::BeginPlay();
	throw std::logic_error("The method or operation is not implemented.");
}

