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
		//서버전용 .  서버를 입력한 경로의 맵으로 이동시킴, 클라이언트랑 같이감 서버가 접속중인 클라이언트들의
		//플레이어 컨트롤러에서 ClientTravel을 호출함.
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
/* 언리얼의 Travel에는 Seamless와 Non-Seamless방식이 있다.
* Seamless = A에서 B맵으로 이동할때 거쳐가는 Transition map을 사용.
* Seamless 방식으로 Map을 이동하면 몇 개의 액터를 지속시킬 수 있다.(게임모드나 컨트롤러같은.)
* Non-Seamless(blocking) = 모든 클라이언트는 접속을 끊은 뒤 서버에 재연걸하여 맵을 새로 로드함.
*/