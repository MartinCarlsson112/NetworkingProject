#include "NPNetDebugWidget.h"
#include "Engine/World.h"
#include "Engine/NetDriver.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Misc/DefaultValueHelper.h"

void UNPNetDebugWidget::UpdateNetworkSimulationSettings(const FNPBlueprintNetworkSimulationSettings &InPackets)
{
	if (UWorld* World = GetWorld())
	{
		if (World->GetNetDriver() != nullptr)
		{
			FPacketSimulationSettings PacketSimulation;
			PacketSimulation.PktLagMin = InPackets.MinLatency;
			PacketSimulation.PktLagMax = InPackets.MaxLatency;
			PacketSimulation.PktLoss = InPackets.PacketLossPercentage;
			PacketSimulation.PktIncomingLagMin = InPackets.MinLatency;
			PacketSimulation.PktIncomingLagMax = InPackets.MaxLatency;
			PacketSimulation.PktIncomingLoss = InPackets.PacketLossPercentage;
			World->GetNetDriver()->SetPacketSimulationSettings(PacketSimulation);

			FNPBlueprintNetworkSimulationSettingsText SimulationSettingsText;
			SimulationSettingsText.MaxLatency = FText::FromString(FString::FromInt(InPackets.MaxLatency));
			SimulationSettingsText.MinLatency = FText::FromString(FString::FromInt(InPackets.MinLatency));
			SimulationSettingsText.PacketLossPercentage = FText::FromString(FString::FromInt(InPackets.PacketLossPercentage));

			BP_OnUpdateNetworkSimulationSettings(SimulationSettingsText);
		}
	}
}

void UNPNetDebugWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APlayerState* PlayerState = PC->GetPlayerState<APlayerState>())
		{
			BP_UpdatePing(static_cast<int32>(PlayerState->GetPing()));
		}
	}
}
