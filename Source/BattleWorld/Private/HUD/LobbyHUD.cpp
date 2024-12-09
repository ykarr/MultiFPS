// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LobbyHUD.h"
#include "UI/Lobby/LobbyWidget.h"
#include "MultiplayerSessions\/Public\MenuWidget.h"
void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(MenuWidgetClass)) {
		MenuWidget = CreateWidget<UMenuWidget>(GetOwningPlayerController(), MenuWidgetClass);
		//ShowHideWidget(EGameWidget::EGW_TeamSelection, true);
		MenuWidget->AddToViewport(0);
		FInputModeUIOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->bShowMouseCursor = true;
	}
}
