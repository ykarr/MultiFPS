// YunMinSeong. All right reserved. 


#include "MenuWidget.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"

#include "Kismet\KismetSystemLibrary.h"
#include "Kismet\GameplayStatics.h"
#include "Engine/AssetManager.h"
//
#include "MultiplayPluginDebugHelper.h"
void UMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//static ConstructorHelpers::FClassFinder<UUserWidget> ServerHostWidgetClassFinder(TEXT("Blueprint'/MultiplayerSessions/Widget/WBP_HostMenu.WBP_HostMenu_C'"));
	//if (ServerHostWidgetClassFinder.Succeeded()) {
	//	ServerHostWidget = ServerHostWidgetClassFinder.Class;
	//}
	if (HostBtn) {
		HostBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnHostButtonClicked);
	}
	if (FindBtn) {
		FindBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnFindGameStarted);
	}
	if (OptionsBtn) {
		OptionsBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnOptionMenu);
	}
	if (QuitBtn) {
		QuitBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnQuitGame);
	}
}
void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;
	UWorld* World = GetWorld();
	if (World) {
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {
			FInputModeUIOnly InputMode;
			//InputMode.SetWidgetToFocus(TakeWidget());
			//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(true);
		}
	}
}
void UMenuWidget::NativeDestruct()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World) {
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {
			FInputModeGameOnly InputMode;
			//PlayerController->SetInputMode(InputMode);
			//PlayerController->bShowMouseCursor = false;
		}
	}
	Super::NativeDestruct();
}
void UMenuWidget::MenuSetup(int32 InNumConnections , FString InMatchType)
{
	MatchType = InMatchType;
	NumConnections = InNumConnections;

	//AddToViewport();
	

	
}



void UMenuWidget::OnHostButtonClicked()
{
	MultiplayDebug::Print("Host");
	if (MultiplayerSessionsSubsystem) {
		//MultiplayerSessionsSubsystem->CreateSession(NumConnections, MatchType);
	}
	RemoveFromParent();
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(ServerHostWidget.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this](){
		if (ServerHostWidget.IsValid()) {
			UUserWidget* HostMenu= CreateWidget(GetWorld()->GetFirstPlayerController(), ServerHostWidget.Get());
			HostMenu->AddToViewport(1);
		}
	}));
}

void UMenuWidget::OnFindGameStarted()
{
	RemoveFromParent();
	//EscapeGameInstance->ShowServerMenu();
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(ServerFindWidget.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this]() {
		if (ServerFindWidget.IsValid()) {
			UUserWidget* FindSessionMenu = CreateWidget(GetWorld()->GetFirstPlayerController(), ServerFindWidget.Get());
			FindSessionMenu->AddToViewport(1);
		}
	}));
}

void UMenuWidget::OnOptionMenu()
{
	RemoveFromParent();
	//EscapeGameInstance->ShowOptionsMenu();
}

void UMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
/*
void UMenuWidget::OnJoinButtonClicked()
{
	MultiplayDebug::Print("Join");
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World) {
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {
			FInputModeGameOnly InputMode;
			//InputMode.SetWidgetToFocus(TakeWidget());
			//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(true);
		}
	}
}

*/
