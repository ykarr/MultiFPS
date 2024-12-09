// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamSelectWidget.generated.h"
class UButton;
/**
 * 
 */
UCLASS()
class BATTLEWORLD_API UTeamSelectWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized()override;
	UFUNCTION()
	void BlueTeamButtonPressed();
	UFUNCTION()
	void RedTeamButtonPressed();
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RedTeamBtn;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BlueTeamBtn;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<AActor> RedCharacter;
	//UPROPERTY(EditAnywhere)
	//TSubclassOf<AActor> BlueCharacter;

};
