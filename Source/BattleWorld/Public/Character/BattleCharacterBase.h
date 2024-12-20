// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/CharacterType.h"
#include "InputActionValue.h"
#include "BattleCharacterBase.generated.h"

//UENUM(BluePrintType)
//enum class EMovementState :uint8 {
//	EMS_Walking UMETA(DisplayName = "Walking"),
//	EMS_Running UMETA(DisplayName = "Runing"),
//	EMS_Crouching UMETA(DisplayName = "Crouching")
//};

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class IPlayerControllerInterface;
class AWeaponBase;
class UDataTable;
class ABattleControllerBase;
class ABattleHUD;
struct FStateData;
class IHUDInterface;
class IGameModeInterface;
class UMainWidget;
class USoundAttenuation;
UCLASS()
class BATTLEWORLD_API ABattleCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	void InitPlayerState();
	UFUNCTION(Client, Reliable)
	void CL_InitOverlay();
	AWeaponBase* DroppedWeaponBase;
	bool bCanLootWeapon;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SprintArmComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> ViewCamera;
	virtual void PossessedBy(AController* NewController) override;
	void AddOverlapWeapon(AWeaponBase* Weapon);
	void SetDroppedWeapon(AWeaponBase* DroppedWeapon);
protected:
	ABattleCharacterBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetBattleCharacterSpeed();
	bool bIsBlueTeam=false;
	UPROPERTY(ReplicatedUsing = ONnRep_PlayerSpawnTransform)
	FTransform PlayerSpawnTransform;
	UFUNCTION()
	void ONnRep_PlayerSpawnTransform();
private:
	UPROPERTY(EditAnywhere)
	//TSoftClassPtr<UMainWidget> OverlayWidgetClass;
	TSubclassOf<UMainWidget> OverlayWidgetClass;
	TObjectPtr<UMainWidget> OverlayWidget;

	ABattleHUD* HUD;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	float MaxHealth;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentAmor)
	float CurrentAmor;
	UPROPERTY(ReplicatedUsing = OnRep_MaxArmor)
	float MaxArmor;

	UFUNCTION()
	void OnRep_MaxHealth();
	UFUNCTION()
	void OnRep_MaxArmor();
	UFUNCTION()
	void OnRep_CurrentHealth();
	
	UFUNCTION()
	void OnRep_CurrentAmor();
	

	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
	TObjectPtr<UDataTable> CharacterDataTable;
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
	USoundAttenuation* SoundAttenuation;
	UPROPERTY(Replicated)
	AWeaponBase* CurrentEquipWeapon; //다른것은 역할이 끝나면 삭제됨. 이것은 삭제하지 않고 유지.
	FTimerHandle ShootWeaponTimerHandle;
	ABattleControllerBase* PlayerController;
	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	UAnimMontage* ReloadAnimMontage;
	UPROPERTY(ReplicatedUsing = OnRep_EquipWeapon)
	AWeaponBase* EquipWeapon;
	UPROPERTY(ReplicatedUsing = OnRep_UnEquipWeapon)
	AWeaponBase* UnEquipWeapon;
	UPROPERTY(ReplicatedUsing = OnRep_LootWeaponBase)
	AWeaponBase* LootWeaponBase;
	UPROPERTY(ReplicatedUsing = OnRep_WeaponToDrop)
	AWeaponBase* WeaponToDrop;
	UPROPERTY(ReplicatedUsing = OnRep_bIsDead)
	bool bIsDead;
	UPROPERTY(Replicated)
	bool bIsReLoading;
	UPROPERTY(Replicated)
	bool bIsEquippingWeapon;
	bool bIsEquipWeaponFound;
	bool bIsOverlapOnce;
	bool bLootingWeapon;
	float ChangeFootStepDelayTime;
	float FootStepDelayTime;
	bool bIsPlayingFootStepSFX;
	float MaxSprintSpeed ;
	float MaxWalkSpeed ;
	float MaxCrouchSpeed;
	UPROPERTY(Replicated)
	bool bIsCrouching;
	UPROPERTY(Replicated)
	bool bIsSprinting;
	UPROPERTY(REplicated)
	float AimDirection;
	UPROPERTY(Replicated,EditAnywhere)
	TArray<EMovementState> BattleMovementState;
	UPROPERTY(Replicated, EditAnywhere)
	EWeaponType EquippedWeaponType;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletonMesh;

	FTimerHandle FootStepTimerHandle;
	UPROPERTY(EditAnywhere, Category = "FootStepSound")
	TArray<TObjectPtr<USoundBase>> FootStepSoundArray;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Context")
	TObjectPtr<UInputMappingContext> BattleContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> CrouchAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> DropAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> LootAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> SelectPrimaryWeaponAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> SelectSecondaryWeaponAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> LeftMouseAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> ReLoadAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> TestAction;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void SetBattleSprint(const FInputActionValue& Value);
	void CustomCrouch(const FInputActionValue& Value);
	void CustomCrouchEnd(const FInputActionValue& Value);
	void SetCrouch(bool bIsCrouch);
	void BattleSprint(bool bIsSprint);
	void CalculateAimDirection();
	void MoveComplete();
	void LootWeapons();
	void SetEquippedWeaponType(EWeaponType WeaponType);
	void DropWeapon(AWeaponBase* WeaponRef);//server Function에서만 실행.
	void SelectPrimaryWeapon();
	void SelectSecondaryWeapon();
	void FireLoop();
	bool HasExtraAmmo();
	bool IsOutOfAmmo();
	void ShootWeapon(const FInputActionValue& Value);
	void SetIsEquipping(bool bIsEquippingWeaponSet);
	UFUNCTION()
	void CallSetIsEquippingFalse();
	UFUNCTION()
	void CallSetIsReloadingWeaponFalse();
	void SetIsReloadingWeapon(bool Value);
	void ReloadingWeapon();
	FStateData* GetCharacterStateData()const;
	
	void SetPlayerHealth(float Value);
	void Death(bool bIsDeathSet);
	void Respawn();
	/*Test함수.*/
	void TestFunc();
	UFUNCTION(Server, Reliable)
	void SRV_TestFunc();
	
	/*Server Play Function*/
	UFUNCTION()
	void OnRep_bIsDead();
	UFUNCTION()
	void OnRep_EquipWeapon();
	UFUNCTION()
	void OnRep_UnEquipWeapon();
	UFUNCTION()
	void OnRep_WeaponToDrop();
	UFUNCTION()
	void OnRep_LootWeaponBase();
	UFUNCTION(Reliable, Server)
	void SRV_CalculateAimDirection();
	UFUNCTION(Reliable, Server)
	void SRV_BattleSprint(bool bIsSprint);
	UFUNCTION(Server, Reliable)
	void SRV_Crouch(bool bIsCrouch);
	UFUNCTION(server, Reliable)
	void SRV_PlayFootStepSFX();
	UFUNCTION(NetMulticast, Reliable)
	void MC_PlayFootStepSFX();
	UFUNCTION(Server, Reliable)
	void SRV_LootWeapons();
	UFUNCTION(Server, Reliable)
	void SRV_SetEquippedWeaponType(EWeaponType WeaponType);
	UFUNCTION(Server, Reliable)
	void SRV_AddOverlapWeapon(AWeaponBase* Weapon);
	UFUNCTION(Server, Reliable)
	void SRV_DropWeapon();
	UFUNCTION(Server, Reliable)
	void SRV_SetDroppedWeapon(AWeaponBase* DroppedWeapon);
	UFUNCTION(Server, Reliable)
	void SRV_RequestLootWeapon();
	UFUNCTION(Server, Reliable)
	void SRV_EquipWeapon(EWeaponType EWeaponEquippedType); //use btn
	UFUNCTION(NetMulticast,Reliable)
	void MC_PlayEquipWeawponMontage(UAnimMontage* WeaponMontage);
	UFUNCTION(Client, Reliable)
	void CL_UpdateInventoryUI(EWeaponType WeaponType, UTexture2D* WeaponThumbnail);
	UFUNCTION(Server, Reliable)
	void SRV_ShootWeapon(bool bIsShooting);
	UFUNCTION(NetMulticast,Reliable)
	void MC_ShootWeaponSFXEFFECT();
	UFUNCTION(Server, Reliable)
	void SRV_SetIsEquipping(bool bIsEquippingWeaponSet);
	UFUNCTION(Client,Reliable)
	void CL_UpdateAmmoUI(int32 CurrentAmmo, int32 ExtraAmmo);
	UFUNCTION(Client,Reliable)
	void CL_DisplayAmmoUI(bool bIsShowWidget);
	UFUNCTION(Server,Reliable)
	void SRV_SetIsReloadingWeapon(bool Value);
	UFUNCTION(NetMulticast,Reliable)
	void MC_SetIsReloadingWeapon(UAnimMontage* ReloadAnim);
	UFUNCTION(NetMulticast,Reliable)
	void MC_BulletImpact(AWeaponBase* Weapon, FHitResult HitResult);
	UFUNCTION(Server, Reliable)
	void SRV_SetPlayerHealth(float Value);
	void SetPlayerAmor(float Value);
	UFUNCTION(Server, Reliable)
	void SRV_SetPlayerAmor(float Value);
	UFUNCTION(Client, Reliable)
	void CL_SetPlayerHealth( );
	UFUNCTION(Client, Reliable)
	void CL_SetPlayerAmor( );
	UFUNCTION(Server,Reliable)
	void SRV_Death(bool bIsDeathSet);
	UFUNCTION(Server, Reliable)
	void SRV_DeathDropWeapon();
	UFUNCTION(Server,Reliable)
	void SRV_Respawn();
public:	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	TScriptInterface<IHUDInterface> BattleHUDInterface;
	TScriptInterface<IGameModeInterface> BattleGameModeInterface;
	//UFUNCTION(Client, Reliable) 
	//void CL_HideHead();
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TArray<TObjectPtr<AWeaponBase>> OverlapWeapons;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TArray<TObjectPtr<AWeaponBase>> WeaponInventory;
	UPROPERTY(VisibleAnywhere, Category = "Interface")
	TScriptInterface<IPlayerControllerInterface> PlayerControllerInterface;
	FORCEINLINE bool GetbIsCrouch() const { return bIsCrouching; };
	FORCEINLINE bool GetbIsSprint() const { return bIsSprinting; };
	FORCEINLINE TArray<EMovementState> GetEMovementState() const { return BattleMovementState; };
	FORCEINLINE EWeaponType GetEquippedWeapon() const { return EquippedWeaponType; };
	FORCEINLINE float GetAimDirection() const { return AimDirection; };

};
