// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BattleHUD.h"
#include "UI/Team/TeamSelectWidget.h"
#include "UI/MainWidget.h"
#include "UI/CrossHair/CrossHairWidget.h"

void ABattleHUD::BeginPlay()
{
	Super::BeginPlay();
	Controller = GetOwningPlayerController();
	if (IsValid(TeamSelectWidgetClass) ) {
		TeamSelectWidget = CreateWidget<UTeamSelectWidget>(Controller, TeamSelectWidgetClass);
		ShowHideWidget(EGameWidget::EGW_TeamSelection, true);
	}
	if (IsValid(OverlayWidgetClass)) {
		//OverlayWidget= CreateWidget<UMainWidget>(Controller, OverlayWidgetClass);

	}
	
	
}

void ABattleHUD::ShowHideWidget(EGameWidget GameWidget, bool bIsCanShow)
{
	CL_ShowHideWidget(GameWidget, bIsCanShow);	
}

void ABattleHUD::UpdateOverlayInventoryUI(EWeaponType WeaponType, UTexture2D* WeaponThumbnail)
{
	//OverlayWidget->UpdateOverlayInventoryUI(WeaponType, WeaponThumbnail);
}

void ABattleHUD::UpdateAmmoUI(int32 CurrentAmmo, int32 ExtraAmmo)
{
	//OverlayWidget->UpdateAmmoUI(CurrentAmmo, ExtraAmmo);
}

void ABattleHUD::DisplayAmmoUI(bool bIsShowWidget)
{
	//OverlayWidget->DisplayAmmoUI(bIsShowWidget);
}

void ABattleHUD::UpdateHealthUI(float CurrentHealth, float MaxHealth)
{
	//OverlayWidget->UpdateHealthUI(CurrentHealth, MaxHealth);
}

void ABattleHUD::UpdateAmorUI(float CurrentAmor, float MaxAmor)
{
	//OverlayWidget->UpdateAmorUI(CurrentAmor, MaxAmor);
}

ABattleHUD* ABattleHUD::GetBattleHUD()
{
	ABattleHUD* BattleHUD=this;
	return BattleHUD;
}


void ABattleHUD::CL_ShowHideWidget_Implementation(EGameWidget GameWidget, bool bIsCanShow)
{
	if (bIsCanShow) {
		switch (GameWidget)
		{
			case EGameWidget::EGW_TeamSelection:
				if (IsValid(TeamSelectWidget)) {
					TeamSelectWidget->AddToViewport(1);
					FInputModeUIOnly InputMode;
					Controller->SetInputMode(InputMode);
					Controller->bShowMouseCursor = true;
				}
				break;
			case EGameWidget::EGW_OverlayWidget:
				if (IsValid(OverlayWidget)) {
					//OverlayWidget = CreateWidget<UMainWidget>(Controller, OverlayWidgetClass);
					//OverlayWidget->AddToViewport(0);
				}
				
			case EGameWidget::EGW_CrossHairWidget:
				if (CrossHairWidget) {
					CrossHairWidget->AddToViewport(0);
				}
				else {
					CrossHairWidget = CreateWidget<UCrossHairWidget>(Controller, CrossHairWidgetClass);
					CrossHairWidget->AddToViewport(0);
				}
				break;
			default:
				break;
		}
		
	}
	else {
		switch (GameWidget)
		{
			case EGameWidget::EGW_TeamSelection:
				if (IsValid(TeamSelectWidget)) {
					if (TeamSelectWidget->IsInViewport()) {
						TeamSelectWidget->RemoveFromParent();
						FInputModeGameOnly InputMode;
						Controller->SetInputMode(InputMode);
						Controller->bShowMouseCursor = false;
					}
					
				}
				break;
			case EGameWidget::EGW_OverlayWidget:
				if (IsValid(OverlayWidget)) {
					//OverlayWidget->RemoveFromParent();
				}
			case EGameWidget::EGW_CrossHairWidget:
				CrossHairWidget->RemoveFromParent();
				break;
			default:
				break;
		}
		
	}
}
