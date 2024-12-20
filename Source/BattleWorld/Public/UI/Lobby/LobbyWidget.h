// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"


class UButton;
class UTextBox;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class BATTLEWORLD_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	//UFUNCTION(BlueprintCallable)
	//void OpenLobby();
	//UFUNCTION(BlueprintCallable)
	//void CallOpenLevel();
	//UFUNCTION(BlueprintCallable)
	//void CallClientTravel();
	//void CallClientTravel(const FString& Address);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> HostBtn;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> JoinBtn;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> IPText;
};
