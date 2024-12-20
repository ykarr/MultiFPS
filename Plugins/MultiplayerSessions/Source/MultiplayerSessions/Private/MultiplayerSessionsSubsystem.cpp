// YunMinSeong. All right reserved. 


#include "MultiplayerSessionsSubsystem.h"
#include "MultiplayPluginDebugHelper.h"
#include <Online\OnlineSessionNames.h>
UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem() :
	OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	OnDestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	OnStartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem) {
		SessionInterface = Subsystem->GetSessionInterface();
	}
}

void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, bool bIsEnableLANMode,FString MatchType)
{
	if (!SessionInterface.IsValid()) {
		MultiplayDebug::Print("Fail CreateGameSession!");
		return;
	}
	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr) {
		//만약 어떤것이 이런 이름으로 존재하고 기존 세션이 nullptr이 아닌경우 현제 세션 파괴
		SessionInterface->DestroySession(NAME_GameSession);
		MultiplayDebug::Print("ExistingSession");
	}
	//OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete);
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	//IOnlineSubsystem::Get()->GetSubsystemName()=="NULL"?true:false;
	SessionSettings->bIsLANMatch = bIsEnableLANMode;			// LAN 연결
	//SessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;	// LAN 연결
	SessionSettings->NumPublicConnections = NumPublicConnections;											// 최대 접속 가능 수
	SessionSettings->bAllowJoinInProgress = true;															// Session 진행중에 접속 허용
	SessionSettings->bAllowJoinViaPresence = true;															// 세션 참가 지역을 현재 지역으로 제한
	SessionSettings->bShouldAdvertise = true;																// 현재 세션을 광고할지
	SessionSettings->bUsesPresence = true;																	// 현재 지역에 세션 표시
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->Set(FName("FreeForAll"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//SessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings)) {
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults, bool bIsEnableLANMode)
{
	if (!SessionInterface.IsValid()) {
		return;
	}
	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 10000;//검색 결과로 나오는 세션 수 최대값
	SessionSearch->bIsLanQuery = bIsEnableLANMode;//LAN 사용 여부
	//SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;//LAN 사용 여부

	//현재 사용자와 연결된 세션을 찾도록 검색 조건을 설정
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef())) {
		MultiplayDebug::Print("fail");
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!SessionInterface.IsValid()) {
		return;
	}
	JoinSessionCompleteDelegateHandle=SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult)) {
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
}

void UMultiplayerSessionsSubsystem::StartSession()
{
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface) {
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}
	if (bWasSuccessful) {
		FString ErrorMsg = TEXT("Session created successfully: ") + SessionName.ToString();
		//MultiplayDebug::Print(ErrorMsg);
		MultiplayDebug::Print(ErrorMsg);
	
		UWorld* World = GetWorld();
		if (World) {
			APlayerController* PlayerController = World->GetFirstPlayerController();
			if (PlayerController) {
				FInputModeGameOnly InputMode;
				//InputMode.SetWidgetToFocus(TakeWidget());
				//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->SetInputMode(InputMode);
				PlayerController->SetShowMouseCursor(false);
			}
			// Script / Engine.World'/Game/Map/BattleMap1.BattleMap1'
			World->ServerTravel("/Game/Map/BattleMap1?listen");
		}
	}
	else {
		MultiplayDebug::Print("CreateSession Fail");

	}
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (SessionInterface) {
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}
	if (SessionSearch->SearchResults.Num() <= 0) {
		MultiplayerOnFindSessionsComplete.Broadcast(SessionSearch->SearchResults, false);
		MultiplayDebug::Print("SearchResults<=0");
		return;
	}
	//if (bWasSuccessful) {
	//	MultiplayerOnFindSessionsComplete.Broadcast(SessionSearch->SearchResults,bWasSuccessful);
	//	//Debug::Print("Find Session Complete");
	//	for (auto Result : SessionSearch->SearchResults) {
	//		FString ID = Result.GetSessionIdStr();
	//		FString User = Result.Session.OwningUserName;
	//		FString MatchType;
	//		Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);
	//		//Debug::Print("ID: " + ID + " User: " + User);
	//		if (MatchType == "FreeForAll") {
	//			//Debug::Print(MatchType);
	//			//OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
	//			//const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	//			//OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
	//		}
	//	}
	//}
	//else {
	//	MultiplayerOnFindSessionsComplete.Broadcast(SessionSearch->SearchResults, bWasSuccessful);
	//}
	MultiplayDebug::Print("SearchResults>0");
	MultiplayerOnFindSessionsComplete.Broadcast(SessionSearch->SearchResults, bWasSuccessful);
}



void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface) {
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}
	FString Address;
	SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
	//SessionInterface->GetResolvedConnectString(InSessionName, Address);

	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PlayerController)
	{
		if (PlayerController) {
			FInputModeGameOnly InputMode;
			//InputMode.SetWidgetToFocus(TakeWidget());
			//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(false);
		}
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName InSessionName, bool bWasSuccessful)
{
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName InSessionName, bool bWasSuccessful)
{
}
