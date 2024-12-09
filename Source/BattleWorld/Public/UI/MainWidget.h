// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/CharacterType.h"
#include "MainWidget.generated.h"


class UImage;
class UTextBlock;
class UOverlay;
class UProgressBar;
class UAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedValue, float, CurrentValue, float, ChangedValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoUpdateValue, int, CurrentValue, int, ChangedValue);
/**
 * 
 */
UCLASS()
class BATTLEWORLD_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct()override;
	virtual void NativeOnInitialized() override;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IMG_Rifle;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IMG_Pistol;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr< UTextBlock> BulletAmountText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr< UTextBlock> HealthText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr< UTextBlock> AmorText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr< UOverlay> BulletOverlay;
	//UPROPERTY(EditDefaultsOnly)
	//TObjectPtr<UTexture2D> WeaponThumbnail;
	void UpdateOverlayInventoryUI(EWeaponType WeaponType, UTexture2D* WeaponThumbnail);
	UFUNCTION()
	void UpdateAmmoUI(int32 CurrentAmmo,int32 ExtraAmmo);
	void DisplayAmmoUI(bool bIsShowWidget);
	UFUNCTION()
	void UpdateHealthUI(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	void UpdateAmorUI(float CurrentAmor, float MaxAmor);
	UPROPERTY(meta=(BindWidget))
	TObjectPtr< UProgressBar> AmorProgressBar;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr< UProgressBar> HealthProgressBar;

	TObjectPtr<UAttributeComponent>AttributeComponent;

	FOnAttributeChangedValue OnHealthChangedValue;
	FOnAttributeChangedValue OnAmorChangedValue;
	FOnAmmoUpdateValue OnAmmoUpdateValue;

};
