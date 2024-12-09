// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/HUDInterface.h"
#include "Character/CharacterType.h"
#include "BattleHUD.generated.h"


UENUM(BlueprintType)
enum class EGameWidget :uint8 {
	EGW_TeamSelection,
	EGW_OverlayWidget,
	EGW_CrossHairWidget
};

class UTeamSelectWidget;
class UMainWidget;
class UCrossHairWidget;

/**
 * 
 */
UCLASS()
class BATTLEWORLD_API ABattleHUD : public AHUD, public IHUDInterface
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void ShowHideWidget(EGameWidget GameWidget,bool bIsCanShow);
	UFUNCTION(Client, Reliable)
	void CL_ShowHideWidget(EGameWidget GameWidget, bool bIsCanShow);
	void UpdateOverlayInventoryUI(EWeaponType WeaponType, UTexture2D* WeaponThumbnail);
	void UpdateAmmoUI(int32 CurrentAmmo, int32 ExtraAmmo);
	void DisplayAmmoUI(bool bIsShowWidget);
	void UpdateHealthUI(float CurrentHealth, float MaxHealth);
	void UpdateAmorUI(float CurrentAmor, float MaxAmor);
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTeamSelectWidget> TeamSelectWidgetClass;
	TObjectPtr<UTeamSelectWidget> TeamSelectWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainWidget> OverlayWidgetClass;
	TObjectPtr<UMainWidget> OverlayWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCrossHairWidget> CrossHairWidgetClass;
	TObjectPtr<UCrossHairWidget> CrossHairWidget;

	// IHUDInterface을(를) 통해 상속됨
	virtual ABattleHUD* GetBattleHUD() override;
private:
	APlayerController* Controller;

	

};
