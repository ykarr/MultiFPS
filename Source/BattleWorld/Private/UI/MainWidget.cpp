// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainWidget.h"
#include "Components/Image.h"
//#include "Character/CharacterType.h"
#include "Character/BattleCharacterBase.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Character\AttributeComponent.h"
#include "Character\BattleCharacterBase.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	OnHealthChangedValue.AddUniqueDynamic(this, &UMainWidget::UpdateHealthUI);
	OnAmmoUpdateValue.AddUniqueDynamic(this, &UMainWidget::UpdateAmmoUI);
	OnAmorChangedValue.AddUniqueDynamic(this, &UMainWidget::UpdateAmorUI);
	//HealthProgressBar->PercentDelegate.BindUFunction(this, "UpdateHealthUI");
	//AmorProgressBar->PercentDelegate.BindUFunction(this, "UpdateAmorUI");
}

void UMainWidget::UpdateOverlayInventoryUI(EWeaponType WeaponType, UTexture2D* WeaponThumbnail) {
	if (IsValid( WeaponThumbnail)) {
		switch (WeaponType)
		{
		case EWeaponType::EWS_UnArm:
			break;
		case EWeaponType::EWS_Pistol:
			IMG_Pistol->SetBrushFromTexture(WeaponThumbnail);
			IMG_Pistol->SetVisibility(ESlateVisibility::HitTestInvisible);
			break;
		case EWeaponType::EWS_Rifle:
			IMG_Rifle->SetBrushFromTexture(WeaponThumbnail);
			IMG_Rifle->SetVisibility(ESlateVisibility::HitTestInvisible);
			break;
		default:
			break;
		}
		/*투명도 조절*/
		FTimerHandle Delay;
		GetWorld()->GetTimerManager().SetTimer(Delay, [this]() {
			switch (Cast<ABattleCharacterBase>(GetOwningPlayerPawn())->GetEquippedWeapon())
			{
			case EWeaponType::EWS_UnArm:
				break;
			case EWeaponType::EWS_Pistol:
				IMG_Pistol->SetOpacity(1);
				IMG_Rifle->SetOpacity(0.25);

				break;
			case EWeaponType::EWS_Rifle:
				IMG_Pistol->SetOpacity(0.25);
				IMG_Rifle->SetOpacity(1);
				break;
			default:
				break;
			}
		}, 0.01, false);
		
	}
	else {
		switch (WeaponType)
		{
		case EWeaponType::EWS_UnArm:
			break;
		case EWeaponType::EWS_Pistol:
			IMG_Pistol->SetVisibility(ESlateVisibility::Collapsed);
			break;
		case EWeaponType::EWS_Rifle:
			IMG_Rifle->SetVisibility(ESlateVisibility::Collapsed);
			break;
		default:
			break;
		}
	}
	
}

void UMainWidget::UpdateAmmoUI(int32 CurrentAmmo, int32 ExtraAmmo)
{
	
	FText Text=FText::Format(
		FText::FromString("{0} / {1}"),
		FText::AsNumber(CurrentAmmo), 
		FText::AsNumber(ExtraAmmo));
	BulletAmountText->SetText(Text);
}

void UMainWidget::DisplayAmmoUI(bool bIsShowWidget)
{
	if (bIsShowWidget) {
		BulletOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else {
		BulletOverlay->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainWidget::UpdateHealthUI(float CurrentHealth,float MaxHealth)
{
	HealthProgressBar->SetPercent(CurrentHealth/ MaxHealth);
	if (HealthText) {
		FText Text = FText::Format(
			FText::FromString("{0} / {1}"),
			FText::AsNumber(CurrentHealth),
			FText::AsNumber(MaxHealth));
		HealthText->SetText(Text);
	}
}

void UMainWidget::UpdateAmorUI(float CurrentAmor, float MaxAmor)
{
	
	AmorProgressBar->SetPercent(CurrentAmor/ MaxAmor);
	if (AmorText) {
		FText Text = FText::Format(
			FText::FromString("{0} / {1}"),
			FText::AsNumber(CurrentAmor),
			FText::AsNumber(MaxAmor));
		AmorText->SetText(Text);
	}
}


