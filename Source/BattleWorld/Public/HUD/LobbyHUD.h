// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

class UMenuWidget;
/**
 * 
 */
UCLASS()
class BATTLEWORLD_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMenuWidget> MenuWidgetClass;
	TObjectPtr<UMenuWidget> MenuWidget;
};
