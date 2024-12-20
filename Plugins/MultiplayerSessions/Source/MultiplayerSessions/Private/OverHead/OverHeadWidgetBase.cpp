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
	Authority = ���� �Ǵ� ���� ���Ͻÿ� �����ϴ� ��.
	Autonomous Proxy = ���� Ŭ���̾�Ʈ���� �����ϰ� �ִ� ���� ���� ����.
	Simulated Proxy = �츮�� �������� �ʴ� Ŭ���̾�Ʈ
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
