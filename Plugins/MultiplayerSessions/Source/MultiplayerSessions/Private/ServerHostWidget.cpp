// YunMinSeong. All right reserved. 


#include "ServerHostWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"
#include "MenuWidget.h"
#include "MultiplayerSessionsSubsystem.h"

void UServerHostWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	EditableServerNameText->OnTextChanged.AddUniqueDynamic(this, &ThisClass::OnEditableServerNameTextChanged);
	BackBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnReturnToMainMenu);
	AcceptBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnAcceptOptions);
	PlayerNumDownBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnDecreasePlayersNum);
	PlayerNumUpBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnIncreasePlayersNum);
	ChooseNetModeLeftBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnChooseNetModeLeft);
	ChooseNetModeRightBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnChooseNetModeRight);
	bIsEnableLANMode = false;
	NetModeText->SetText(FText::FromString("INTERNET"));
}

void UServerHostWidget::NativeConstruct()
{
	Super::NativeConstruct();
	EditableServerNameText->SetText(FText::GetEmpty());
	//AcceptBtn->SetIsEnabled(false);

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance) {
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}
}

void UServerHostWidget::OnEditableServerNameTextChanged(const FText& InText)
{
	/*
	ServerNameText = InText;
	if (!InText.IsEmpty()) {
		//bActivateUseAcceptBtn = true;
		AcceptBtn->SetIsEnabled(true);
	}
	else {
		//bActivateUseAcceptBtn = false;
		AcceptBtn->SetIsEnabled(false);
	}*/
}

void UServerHostWidget::OnReturnToMainMenu()
{
	RemoveFromParent();
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(MainMenuWidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this]() {
		if (MainMenuWidgetClass.IsValid()) {
			UUserWidget* HostMenu = CreateWidget(GetWorld()->GetFirstPlayerController(), MainMenuWidgetClass.Get());
			HostMenu->AddToViewport(1);
			Cast<UMenuWidget>(HostMenu)->MenuSetup();
		}
	}));
}

void UServerHostWidget::OnAcceptOptions()
{
	RemoveFromParent();
	//UE_LOG(LogTemp, Display, TEXT("asdfsadfasdfasdfasdf"));
	MultiplayerSessionsSubsystem->CreateSession(NumberOfPlayers, bIsEnableLANMode,"FreeForAll");
}

void UServerHostWidget::OnDecreasePlayersNum()
{
	NumberOfPlayers = FMath::Clamp(NumberOfPlayers - 1, 2, MaxNumberOfPlayers);
	FString TmpString = FString::FromInt(NumberOfPlayers);
	FText TmpText = FText::FromString(TmpString);
	NumPlayerText->SetText(TmpText);
}

void UServerHostWidget::OnIncreasePlayersNum()
{
	NumberOfPlayers = FMath::Clamp(NumberOfPlayers + 1, 2, MaxNumberOfPlayers);
	FString TmpString = FString::FromInt(NumberOfPlayers);
	FText TmpText = FText::FromString(TmpString);
	NumPlayerText->SetText(TmpText);
}

void UServerHostWidget::OnChooseNetModeLeft()
{
	if (bIsEnableLANMode) {
		//bIsEnableLANMode=false;
	}
	else {
		bIsEnableLANMode = true;
		NetModeText->SetText(FText::FromString("LAN"));
	}
}

void UServerHostWidget::OnChooseNetModeRight()
{
	if (bIsEnableLANMode) {
		bIsEnableLANMode = false;
		NetModeText->SetText(FText::FromString("INTERNET"));
	}
	else {
		//bIsEnableLANMode = true;
	}
}
