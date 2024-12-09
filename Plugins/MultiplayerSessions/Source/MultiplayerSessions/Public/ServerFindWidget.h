// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
//#include "Interfaces/OnlineSessionInterface.h"
#include "ServerFindWidget.generated.h"

#define RESET_WAITTIME 10
class UWidgetSwitcher;
class UButton;
class UTextBlock;
class UMultiplayerSessionsSubsystem;
//class FOnlineSessionSearchResult;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UServerFindWidget : public UUserWidget
{
	GENERATED_BODY()
public:
protected:
	virtual void NativeOnInitialized()override;
	//virtual void NativePreConstruct()override;
	virtual void NativeConstruct()override;
	//virtual void NativeDestruct()override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ToggleDisplayed;
	UPROPERTY(meta = (BindWidget))
	UButton* ChooseNetModeLeftBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* ChooseNetModeRightBtn;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NetModeText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DisplayText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountDownText;
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* AcceptBtn;
private:
	int32 WaitTime = 10;
	FText PlayModeText;
	bool bEnableLan;
	bool bSessionFound;
	UFUNCTION()
	void OnNetSelectLeftBtnClicked();
	UFUNCTION()
	void OnNetSelectRightBtnClicked();
	UFUNCTION()
	void OnBackGameBtn();
	UFUNCTION()
	void OnAcceptGame();
	void DisplaySession(const FOnlineSessionSearchResult& InSessionToJoin);
	void RefreshServers();
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> MainMenuWidgetClass;
	FOnlineSessionSearchResult SessionToJoin;
	FTimerHandle DelayHandle;
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};
