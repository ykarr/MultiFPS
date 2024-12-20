// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class UMultiplayerSessionsSubsystem;
class UButton;
class UServerHostWidget;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 InNumConnections = 4, FString InMatchType = "FreeForAll");
protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct()override;
	virtual void NativeDestruct() override;

	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UPROPERTY()
	int32 NumConnections;
	UPROPERTY()
	FString MatchType ;

	//virtual void NativeOnInitialized() override;
	//virtual void NativePreConstruct();
	//virtual void NativeConstruct() override;
	//virtual void NativeDestruct();
	UPROPERTY(meta = (bindWidget))
	UButton* HostBtn;
	UPROPERTY(meta = (bindWidget))
	UButton* FindBtn;
	UPROPERTY(meta = (bindWidget))
	UButton* OptionsBtn;
	UPROPERTY(meta = (bindWidget))
	UButton* QuitBtn;
private:
	UFUNCTION()
	void OnHostButtonClicked();
	UFUNCTION()
	void OnFindGameStarted();
	UFUNCTION()
	void OnOptionMenu();
	UFUNCTION()
	void OnQuitGame();
	
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> ServerHostWidget;
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> ServerFindWidget;
	//TSoftClassPtr<UServerHostWidget> ServerHostWidget;
};
