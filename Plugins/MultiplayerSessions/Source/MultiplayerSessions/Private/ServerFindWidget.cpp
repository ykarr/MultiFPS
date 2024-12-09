// YunMinSeong. All right reserved. 


#include "ServerFindWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components\Button.h"
#include "Engine/AssetManager.h"
#include "MenuWidget.h"
#include "MultiplayerSessionsSubsystem.h"

//#include "OnlineSessionSettings.h"
//#include "Interfaces/OnlineSessionInterface.h"
//
#include "MultiplayPluginDebugHelper.h"
void UServerFindWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ChooseNetModeLeftBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnNetSelectLeftBtnClicked);
	ChooseNetModeRightBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnNetSelectRightBtnClicked);

	BackBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackGameBtn);
	AcceptBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnAcceptGame);
	//PlayModeText = FText::FromString("LAN");
	NetModeText->SetText(FText::FromString("INTERNET"));
	bEnableLan = false;

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance) {
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
	}
}

void UServerFindWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UServerFindWidget::OnNetSelectLeftBtnClicked()
{
	if (bEnableLan) {

	}
	else {
		bEnableLan = true;
		NetModeText->SetText(FText::FromString("LAN"));
	}
}

void UServerFindWidget::OnNetSelectRightBtnClicked()
{
	if (bEnableLan) {
		bEnableLan = false;
		NetModeText->SetText(FText::FromString("INTERNET"));

	}
}

void UServerFindWidget::OnBackGameBtn()
{
	RemoveFromParent();
	//EscapeGameInstance->ShowLobbyWidget();
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(MainMenuWidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this]() {
		if (MainMenuWidgetClass.IsValid()) {
			UUserWidget* HostMenu = CreateWidget(GetWorld()->GetFirstPlayerController(), MainMenuWidgetClass.Get());
			HostMenu->AddToViewport(1);
			Cast<UMenuWidget>(HostMenu)->MenuSetup();
		}
	}));
	RefreshServers();
}

void UServerFindWidget::OnAcceptGame()
{
	//Test
	
	//bSessionFound = true;
	if (bSessionFound) {
		GetWorld()->GetTimerManager().ClearTimer(DelayHandle);
		RemoveFromParent();
		MultiplayerSessionsSubsystem->JoinSession(SessionToJoin);
		//EscapeGameInstance->JoinGameSession(bEnableLan, SessionToJoin);
	}
	else {
		//RefreshServers();
		RefreshServers();
	}
}

void UServerFindWidget::DisplaySession(const FOnlineSessionSearchResult& InSessionToJoin)
{
	CountDownText->SetText(FText::FromString(FString::FromInt(WaitTime)));
	GetWorld()->GetTimerManager().ClearTimer(DelayHandle);
	//SessionToJoin=InSessionToJoin;
	FString ID;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([this,&InSessionToJoin]() {
		if (bSessionFound) {
			FString ID = InSessionToJoin.GetSessionIdStr();
			//MultiplayDebug::Print("ID: " + ID);
			WaitTime -= 1;
			CountDownText->SetText(FText::FromString(FString::FromInt(WaitTime)));
			
			if (0 == WaitTime) {
				//CountDown End 
				//Reset
				ToggleDisplayed->SetActiveWidgetIndex(0);
				bSessionFound = false;
				WaitTime = RESET_WAITTIME;
				//SessionToJoin = FOnlineSessionSearchResult();
				//ButtonVisibility = ESlateVisibility::Visible;
				DisplayText->SetText(FText::FromString("Choose Search Method"));
				GetWorld()->GetTimerManager().ClearTimer(DelayHandle);
			}
		}
		else {
			GetWorld()->GetTimerManager().ClearTimer(DelayHandle);
		}
	}), 1, true);
	DisplayText->SetText(FText::FromString("Match Avaliable. Join the Match?"));
}

void UServerFindWidget::RefreshServers()
{
	//ButtonVisibility = ESlateVisibility::Hidden;
	DisplayText->SetText(FText::FromString("Searching..."));
	ToggleDisplayed->SetActiveWidgetIndex(1);
	//SessionToJoins.Empty();

	//OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UEscapeGameInstance::OnFindSessionComplete);
	//SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
	//OnFindSessionsCompleteDelegate.
	//TSharedPtr<FOnlineSessionSearch> SessionSearch;
	//// Find Game Session
	//SessionSearch = MakeShareable(new FOnlineSessionSearch());
	//SessionSearch->MaxSearchResults = 20;	// 검색 결과로 나오는 세션 수 최대값
	//SessionSearch->bIsLanQuery = bEnableLan;			// LAN 사용 여부
	//SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); // 찾을 세션 쿼리를 현재로 설정한다

	//const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	////OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
	//bool SearchEndSUCCESS = EscapeGameInstance->SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
	MultiplayerSessionsSubsystem->FindSessions(1000, bEnableLan);
	
}

void UServerFindWidget::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	
	if (bWasSuccessful) {
		MultiplayDebug::Print("findSuccess");
		bSessionFound = true;
		ToggleDisplayed->SetActiveWidgetIndex(2);
		SessionToJoin = SessionResults[0];
		for (auto SessionResult:SessionResults) {
			//const FOnlineSessionSettings& Settings = SessionResult.Session.SessionSettings;
			//Settings.
			//FString ID = SessionResult.Session.SessionSettings.
			//SessionResult.Session.SessionSettings.
			//MultiplayDebug::Print("ID: " + );
		}
		DisplaySession(SessionResults[0]);
		//for (const FOnlineSessionSearchResult& SearchResult : SessionResults) {
		//	//SearchResult.Session.SessionSettings.
		//	//		FString User = Result.Session.OwningUserName;
		//	//		FString MatchType;
		//	//		Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);
		//	//		//Debug::Print("ID: " + ID + " User: " + User);
		//	FString ID = SearchResult.GetSessionIdStr();
		//	MultiplayDebug::Print("ID: "+ID);
		//}
	}
	else {
		MultiplayDebug::Print("findFail");
		bSessionFound = false;
		DisplayText->SetText(FText::FromString("No Sessions Found"));
		//ButtonVisibility = ESlateVisibility::Visible;
		ToggleDisplayed->SetActiveWidgetIndex(0);
	}
}
