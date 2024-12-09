// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Character/CharacterType.h"
#include "ItemDataTable.generated.h"


USTRUCT()
struct FItemData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
	FItemData(){};
	/*String을 사용하면 향후 번역 불가.*/
	UPROPERTY(EditAnywhere)
	FText ItemName;
	UPROPERTY(EditAnywhere)
	EWeaponType Type;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Thumbnail;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ItemClassRef;
	UPROPERTY(EditAnywhere)
	float ShootingSpeed;
	UPROPERTY(EditAnywhere)
	USoundBase* ShootingSFX;
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
	UAnimMontage* EquipAnimation;
	UPROPERTY(EditAnywhere)
	UAnimMontage* ReloadAnimation;
	UPROPERTY(EditAnywhere)
	UAnimMontage* FireAnimation;
	UPROPERTY(EditAnywhere)
	int32 MaxAmmoAmount;
	UPROPERTY(EditAnywhere)
	int32 MaxExtraAmmo;
	UPROPERTY(EditAnywhere)
	USoundBase* OutOfAmmoSFX;
	UPROPERTY(EditAnywhere)
	USoundBase* BulletImpactSFX;
	UPROPERTY(EditAnywhere)
	UParticleSystem* BulletImpactVFX;
	UPROPERTY(EditAnywhere)
	USoundBase* BulletPlayerImpactSFX;
	UPROPERTY(EditAnywhere)
	UParticleSystem* BulletPlayerImpactVFX;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* BulletDecal;
	UPROPERTY(EditAnywhere)
	float HeadDamage;
	UPROPERTY(EditAnywhere)
	float BodyDamage;
};

UCLASS()
class BATTLEWORLD_API UItemDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
