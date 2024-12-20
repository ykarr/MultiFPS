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
		//���� ����� �̷� �̸����� �����ϰ� ���� ������ nullptr�� �ƴѰ�� ���� ���� �ı�
		SessionInterface->DestroySession(NAME_GameSession);
		MultiplayDebug::Print("ExistingSession");
	}
	//OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete);
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	//IOnlineSubsystem::Get()->GetSubsystemName()=="NULL"?true:false;
	SessionSettings->bIsLANMatch = bIsEnableLANMode;			// LAN ����
	//SessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;	// LAN ����
	SessionSettings->NumPublicConnections = NumPublicConnections;											// �ִ� ���� ���� ��
	SessionSettings->bAllowJoinInProgress = true;															// Session �����߿� ���� ���
	SessionSettings->bAllowJoinViaPresence = true;															// ���� ���� ������ ���� �������� ����
	SessionSettings->bShouldAdvertise = true;																// ���� ������ ��������
	SessionSettings->bUsesPresence = true;																	// ���� ������ ���� ǥ��
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
	SessionSearch->MaxSearchResults = 10000;//�˻� ����� ������ ���� �� �ִ밪
	SessionSearch->bIsLanQuery = bIsEnableLANMode;//LAN ��� ����
	//SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;//LAN ��� ����

	//���� ����ڿ� ����� ������ ã���� �˻� ������ ����
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
