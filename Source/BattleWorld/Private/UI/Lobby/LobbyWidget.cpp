// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
void ULobbyWidget::NativeOnInitialized()
{
	//HostBtn->OnPressed.AddDynamic(this, &ULobbyWidget::OpenLobby);
	//JoinBtn->OnPressed.AddDynamic(this, &ULobbyWidget::CallClientTravel);
}
void ULobbyWidget::NativeConstruct()
{
	
}
/*
void ULobbyWidget::OpenLobby()
{
	if (GetWorld()) {
		UE_LOG(LogTemp, Display, TEXT("OpenLobby"));
		//�������� .  ������ �Է��� ����� ������ �̵���Ŵ, Ŭ���̾�Ʈ�� ���̰� ������ �������� Ŭ���̾�Ʈ����
		//�÷��̾� ��Ʈ�ѷ����� ClientTravel�� ȣ����.
		GetWorld()->ServerTravel("/Game/Map/BattleMap1?listen");
		//GetWorld()->ServerTravel("/Game/Map/BattleMain?listen");
	}
}

//void ULobbyWidget::CallOpenLevel(const FString& Address)
void ULobbyWidget::CallOpenLevel()
{
	UE_LOG(LogTemp, Display, TEXT("OpenLevel"));
	
	//IPText->GetText
	UGameplayStatics::OpenLevel(this, FName((IPText->GetText().ToString())));
}

//void ULobbyWidget::CallClientTravel(const FString& Address)
void ULobbyWidget::CallClientTravel()
{
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PlayerController) {
		PlayerController->ClientTravel(IPText->GetText().ToString(), ETravelType::TRAVEL_Absolute);

	}
}*/
/* �𸮾��� Travel���� Seamless�� Non-Seamless����� �ִ�.
* Seamless = A���� B������ �̵��Ҷ� ���İ��� Transition map�� ���.
* Seamless ������� Map�� �̵��ϸ� �� ���� ���͸� ���ӽ�ų �� �ִ�.(���Ӹ�峪 ��Ʈ�ѷ�����.)
* Non-Seamless(blocking) = ��� Ŭ���̾�Ʈ�� ������ ���� �� ������ �翬���Ͽ� ���� ���� �ε���.
*/