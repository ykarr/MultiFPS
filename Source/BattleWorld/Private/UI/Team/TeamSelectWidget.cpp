// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Team/TeamSelectWidget.h"
#include "Components/Button.h"
#include "Character/BattleControllerBase.h"
void UTeamSelectWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	RedTeamBtn->OnPressed.AddDynamic(this, &ThisClass::RedTeamButtonPressed);
	BlueTeamBtn->OnPressed.AddDynamic(this, &ThisClass::BlueTeamButtonPressed);
}

void UTeamSelectWidget::BlueTeamButtonPressed()
{
	//UE_LOG(LogTemp, Display, TEXT("Blue"));
	//Cast<ABattleControllerBase>(GetOwningPlayer())->SelectTeam(BlueCharacter);
	Cast<ABattleControllerBase>(GetOwningPlayer())->SelectTeam(true);
}

void UTeamSelectWidget::RedTeamButtonPressed()
{
	//UE_LOG(LogTemp, Display, TEXT("Red"));
	//Cast<ABattleControllerBase>(GetOwningPlayer())->SelectTeam(RedCharacter);
	Cast<ABattleControllerBase>(GetOwningPlayer())->SelectTeam(false);
}
