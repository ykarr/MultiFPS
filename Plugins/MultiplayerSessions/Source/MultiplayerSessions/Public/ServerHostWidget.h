// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerHostWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;
class UMultiplayerSessionsSubsystem;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UServerHostWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized()override;
	//virtual void NativePreConstruct()override;
	virtual void NativeConstruct()override;
	//virtual void NativeDestruct()override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* EditableServerNameText;
	UPROPERTY(meta = (BindWidget))
	UButton* PlayerNumDownBtn;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NumPlayerText;
	UPROPERTY(meta = (BindWidget))
	UButton* PlayerNumUpBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* ChooseNetModeLeftBtn;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NetModeText;
	UPROPERTY(meta = (BindWidget))
	UButton* ChooseNetModeRightBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* AcceptBtn;


private:
	UFUNCTION()
	void OnEditableServerNameTextChanged(const FText& InText);
	UFUNCTION()
	void OnReturnToMainMenu();
	UFUNCTION()
	void OnAcceptOptions();
	UFUNCTION()
	void OnDecreasePlayersNum();
	UFUNCTION()
	void OnIncreasePlayersNum();
	UFUNCTION()
	void OnChooseNetModeLeft();
	UFUNCTION()
	void OnChooseNetModeRight();
	FText ServerNameText;
	bool bActivateUseAcceptBtn = false;
	int32 NumberOfPlayers = 2;
	int32 MaxNumberOfPlayers = 8;
	bool bIsEnableLANMode = true;

	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> MainMenuWidgetClass;
};
