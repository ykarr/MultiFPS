// YunMinSeong. All right reserved. 


#include "OverHead/OverHeadWidgetBase.h"
#include "Components/TextBlock.h"
void UOverHeadWidgetBase::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText) {
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverHeadWidgetBase::ShowPlayerNetRole(APawn* InPawn)
{
	FString PlayerName = "";
	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;
	switch (RemoteRole) {
	case ENetRole::ROLE_Authority:
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("AutonomousProxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("SimulatedProxy");
		break;
	case ENetRole::ROLE_None:
		Role = FString("None");
		break;
	}
	/*
	Authority = 서버 또는 자율 프록시에 존재하는 폰.
	Autonomous Proxy = 현재 클라이언트에서 제어하고 있는 폰의 로컬 역할.
	Simulated Proxy = 우리가 통제하지 않는 클라이언트
	*/
	FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s\n%s"), *Role, *PlayerName);

	SetDisplayText(RemoteRoleString);
}

void UOverHeadWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	ShowPlayerNetRole(GetOwningPlayerPawn());
}

void UOverHeadWidgetBase::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
