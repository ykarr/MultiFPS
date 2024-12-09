// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/CharacterType.h"
#include "Interface/WeaponInterface.h"
#include "WeaponBase.generated.h"
class USphereComponent;
class UDataTable;
class ABattleCharacterBase;
struct FItemData;
UCLASS()
class BATTLEWORLD_API AWeaponBase : public AActor,public IWeaponInterface
{
	GENERATED_BODY()
	
public:
	
	FItemData* GetItemData();

	//UPROPERTY(EditAnywhere,Replicated)
	//EWeaponType WeaponType;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> Sphere;
	bool bCanbeInteracted;//서버를 통해 실행할 예정이라 복제할필요없음.
	UPROPERTY(Replicated)
	int32 CurrentAmmo;
	UPROPERTY(Replicated)
	int32 CurrentMaxExtraAmmo;

protected:
	AWeaponBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	FTimerHandle OverlapDelay;
private:
	UPROPERTY(EditDefaultsOnly,Category = "WeaponData")
	FString WeaponName;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	TObjectPtr<UDataTable> WeaponDataTable;
	FItemData* WeaponInfo;
	ABattleCharacterBase* BattleCharacterRef;
	void BeginOverlapAction(AActor* OtherActor);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void TempFunc();
	UFUNCTION(Server,Reliable)
	void SRV_InitWeapon();

	// IWeaponInterface을(를) 통해 상속됨
	AWeaponBase* GetWeaponRef() override;

};
