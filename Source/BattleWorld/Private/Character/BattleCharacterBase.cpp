//Ctrl M + Ctrl O ??? ????. Ctrl M + Ctrl L ??? ????.

#include "Character/BattleCharacterBase.h"
/*GameMode*/
#include "GameMode/BattleGameModeBase.h"
/*Replicated*/
#include "Net/UnrealNetwork.h"
/*UI*/
#include "HUD/BattleHUD.h"
#include "UI/MainWidget.h"
/*Components*/
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components\SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/AttributeComponent.h"
/*Enhanced Input*/
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
/*Weapon*/
#include "Items/Weapons/WeaponBase.h"
#include "Items/Data/ItemDataTable.h"
#include "Components/DecalComponent.h"
/*Character*/
#include "Engine/SkeletalMeshSocket.h"
#include "Interface/PlayerControllerInterface.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Character/BattleControllerBase.h"
#include "Animation\AnimMontage.h"
#include "Camera/PlayerCameraManager.h"
#include "Character\CharacterStateDataTable.h"
/*Etc*/
#include "Kismet/GameplayStatics.h"
#include "Kismet\KismetMathLibrary.h"
//#include "Engine/AssetManager.h"
/*ĳ???? ????? ??????? ????? ????? ??? ??????????? ??????? ?????? ????.
*??? ???????? ?? ??? Mesh?? ????? ?? Mesh?? ??? Player???? ??????? ????.
*????? ??????? ?ι??????? ?????? ????.*/

ABattleCharacterBase::ABattleCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCapsuleRadius(35.f);
	GetMesh()->SetRelativeLocation(FVector(5.f, 0.f, -90.f));
	GetMesh()->bOnlyOwnerSee = true;
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	//GetMesh()->SetNotifyRigidBodyCollision(true);//Simulation Generate Hit Events ??? ?? ???°??? ??? ?? ????? ??°???...?

	SkeletonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SecondMesh"));
	SkeletonMesh->SetupAttachment(GetMesh());
	//SkeletonMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	//SkeletonMesh->bHiddenInGame=true;
	SkeletonMesh->bOwnerNoSee = true;
	//SkeletonMesh->SetCollisionEnabled(ECollisionEnabled::);
	/**
	*	If true, the primitive will cast shadows even if bHidden is true.
	*	Controls whether the primitive should cast shadows when hidden.
	*	This flag is only used if CastShadow is true.
	*/
	SkeletonMesh->bCastHiddenShadow = true;
	SprintArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
	SprintArmComponent->bUsePawnControlRotation = true;
	SprintArmComponent->SetupAttachment(GetMesh(),FName("Head_Camera"));
	SprintArmComponent->TargetArmLength = 0.f;
	SprintArmComponent->bEnableCameraRotationLag = true;
	SprintArmComponent->CameraRotationLagSpeed = 90.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SprintArmComponent);


	FootStepDelayTime = 0.5;
	ChangeFootStepDelayTime = 0.5;
	MaxSprintSpeed = 600.f;
	MaxWalkSpeed = 250.f;
	MaxCrouchSpeed = 150.f;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	BattleMovementState.Add(EMovementState::EMS_Walking);
	bIsPlayingFootStepSFX = false;

	EquippedWeaponType = EWeaponType::EWS_UnArm;
	bCanLootWeapon = true;
	bIsOverlapOnce = true;
	bIsReLoading = false;
	

}
void ABattleCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		UE_LOG(LogTemp, Display, TEXT("BeginPlay--"));
	}
	else {

		UE_LOG(LogTemp, Display, TEXT("BeginPlay++"));
	}
	Tags.Add("Player");
	FTimerHandle StartDelay;
	GetWorld()->GetTimerManager().SetTimer(StartDelay, [this]() {
		PlayerController = Cast<ABattleControllerBase>(GetController());
		if (PlayerController) {
			if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				Subsystem->AddMappingContext(BattleContext, 0);
			}
		}
	}, 0.1, false);
	GetMesh()->HideBoneByName(FName("head"), EPhysBodyOp::PBO_None);
	/*FTimerHandle StateInitDelay;
	GetWorld()->GetTimerManager().SetTimer(StateInitDelay, [this]() {
		InitPlayerState();
	}, 1, false);*/
		
}

void ABattleCharacterBase::CL_InitOverlay_Implementation()
{
	//APlayerController* PlayerControllerBattle = Cast<APlayerController>(Controller);
	PlayerController = Cast<ABattleControllerBase>(GetController());
	if (!OverlayWidget) {
		OverlayWidget = CreateWidget<UMainWidget>(PlayerController, OverlayWidgetClass);
	}
	if (OverlayWidget) {
		OverlayWidget->AddToViewport(0);
	}
}

void ABattleCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}
void ABattleCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABattleCharacterBase::TestFunc()
{
	if (HasAuthority()) {
		UKismetSystemLibrary::PrintString(GetWorld(), FString("TestFunc Excute"));
	}
}
void ABattleCharacterBase::SRV_TestFunc_Implementation()
{
	if (HasAuthority()) {
		UKismetSystemLibrary::PrintString(GetWorld(), FString("SRV_TestFunc Excute"));
	}
}

#pragma region Attribute
void ABattleCharacterBase::InitPlayerState()
{
	

	CL_InitOverlay();
	if (const FStateData* StateData = GetCharacterStateData()) {
		MaxHealth = StateData->MaxHealth;
		OnRep_MaxHealth();
		MaxArmor = StateData->MaxArmor;
		OnRep_MaxArmor();
	}
}
void ABattleCharacterBase::OnRep_MaxHealth()
{
	SetPlayerHealth(MaxHealth);
}
void ABattleCharacterBase::OnRep_MaxArmor()
{
	SetPlayerAmor(MaxArmor);
}
FStateData* ABattleCharacterBase::GetCharacterStateData() const
{
	return CharacterDataTable->FindRow<FStateData>(FName("Player"), FString("Player"));
}
void ABattleCharacterBase::CL_UpdateAmmoUI_Implementation(int32 CurrentAmmo, int32 ExtraAmmo)
{
	OverlayWidget->OnAmmoUpdateValue.Broadcast(CurrentAmmo, ExtraAmmo);
}
void ABattleCharacterBase::CL_DisplayAmmoUI_Implementation(bool bIsShowWidget)
{
	OverlayWidget->DisplayAmmoUI(bIsShowWidget);
	//PlayerController->BattleHUD->DisplayAmmoUI(bIsShowWidget);
}
void ABattleCharacterBase::CL_SetPlayerHealth_Implementation()
{
	if (OverlayWidget) {
		OverlayWidget->OnHealthChangedValue.Broadcast(CurrentHealth, MaxHealth);
	}
	//OverlayWidget->UpdateHealthUI(CurrentHealth, MaxHealth);
}
void ABattleCharacterBase::CL_SetPlayerAmor_Implementation()
{
	FStateData* StateData = GetCharacterStateData();
	if (OverlayWidget) {
		OverlayWidget->OnAmorChangedValue.Broadcast(CurrentAmor, StateData->MaxArmor);
	}

}
void ABattleCharacterBase::SetPlayerHealth(float Value)
{
	SRV_SetPlayerHealth(Value);

}
void ABattleCharacterBase::SRV_SetPlayerHealth_Implementation(float Value)
{
	UKismetSystemLibrary::PrintString(GetWorld(),FString::FormatAsNumber(Value));
	CurrentHealth = Value;
	OnRep_CurrentHealth();
}
void ABattleCharacterBase::SetPlayerAmor(float Value)
{
	SRV_SetPlayerAmor(Value);
}
void ABattleCharacterBase::SRV_SetPlayerAmor_Implementation(float Value)
{
	CurrentAmor = Value;
	OnRep_CurrentAmor();
}

void ABattleCharacterBase::OnRep_CurrentHealth()
{
	CL_SetPlayerHealth();
}

void ABattleCharacterBase::OnRep_CurrentAmor()
{
	CL_SetPlayerAmor();
}

#pragma endregion
#pragma region Attack
void ABattleCharacterBase::MC_ShootWeaponSFXEFFECT_Implementation()
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	UAnimInstance* SkeletonAnim = SkeletonMesh->GetAnimInstance();
	if (CurrentEquipWeapon->CurrentAmmo > 0) {
		Anim->Montage_Play(CurrentEquipWeapon->GetItemData()->FireAnimation);
		SkeletonAnim->Montage_Play(CurrentEquipWeapon->GetItemData()->FireAnimation);
		UGameplayStatics::SpawnEmitterAttached(CurrentEquipWeapon->GetItemData()->MuzzleFlash, CurrentEquipWeapon->SkeletalMeshComponent, FName("S_MuzzleFlash"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
		UGameplayStatics::PlaySoundAtLocation(this, CurrentEquipWeapon->GetItemData()->ShootingSFX, GetActorLocation());
	}
	else {
		//UGameplayStatics::PlaySoundAtLocation(this, CurrentEquipWeapon->GetItemData()->OutOfAmmoSFX, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(this, CurrentEquipWeapon->GetItemData()->OutOfAmmoSFX, GetActorLocation(), 1.0f, 1.0f, 0.f, SoundAttenuation);
	}
}
void ABattleCharacterBase::FireLoop()
{
	FHitResult Hit;
	FVector Loc = ViewCamera->GetComponentLocation();
	FVector Rot = ViewCamera->GetForwardVector();
	//DrawDebugLine(GetWorld(), ViewLoc, ViewLoc+(ViewRot*10000), FColor::Red, false, 10,0,1);
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, this);
	TArray<AActor*> Ac;
	Ac.Add(this);
	MC_ShootWeaponSFXEFFECT();
	
	float RandomX = FMath::FRandRange(-100.f, 100.f);
	float RandomY = FMath::FRandRange(-100.f, 100.f);
	float RandomZ = FMath::FRandRange(-100.f, 100.f);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Loc, Loc + FVector(RandomX, RandomY, RandomZ) + (Rot * 10000), ECollisionChannel::ECC_Visibility, CollisionQueryParams)) {
		//if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Loc, (Rot * 10000) + Loc, ETraceTypeQuery::TraceTypeQuery1, false, Ac, EDrawDebugTrace::ForDuration, Hit, true)) {
		if (Hit.bBlockingHit&&!IsOutOfAmmo()) {
			UE_LOG(LogTemp, Display, TEXT("Hit"));
			MC_BulletImpact(CurrentEquipWeapon, Hit);
			if (Hit.GetActor()->ActorHasTag("Player")) {
				float ShootDmg;
				if (Hit.BoneName.ToString() == "Head") {
					UE_LOG(LogTemp, Display, TEXT("head"));
					ShootDmg = CurrentEquipWeapon->GetItemData()->HeadDamage;
				}
				else {
					UE_LOG(LogTemp, Display, TEXT("body"));
					ShootDmg = CurrentEquipWeapon->GetItemData()->BodyDamage;
				}
				if (GEngine) {
					const FString FinalMSG = "ShootDmg: "+ FString::SanitizeFloat(ShootDmg);
					GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::MakeRandomColor(), FinalMSG, true);
					UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMSG);
				}
				UGameplayStatics::ApplyDamage(Hit.GetActor(), ShootDmg, GetInstigator()->GetController(), this, UDamageType::StaticClass());
			}

		}
	}
	//탄약 업데이트
	CurrentEquipWeapon->CurrentAmmo = FMath::Clamp(CurrentEquipWeapon->CurrentAmmo - 1, 0, CurrentEquipWeapon->GetItemData()->MaxAmmoAmount);
	CL_UpdateAmmoUI(CurrentEquipWeapon->CurrentAmmo, CurrentEquipWeapon->CurrentMaxExtraAmmo);

	if (IsOutOfAmmo() && !bIsReLoading && HasExtraAmmo()) {
		SetIsReloadingWeapon(true);
	}
}
bool ABattleCharacterBase::HasExtraAmmo()
{
	if (CurrentEquipWeapon) {
		return CurrentEquipWeapon->CurrentMaxExtraAmmo != 0;
	}
	return false;
}
bool ABattleCharacterBase::IsOutOfAmmo()
{
	if (CurrentEquipWeapon) {
		return CurrentEquipWeapon->CurrentAmmo <= 0;
	}
	return false;
}
void ABattleCharacterBase::SRV_ShootWeapon_Implementation(bool bIsShooting)
{
	if (bIsShooting && !bIsEquippingWeapon && !bIsReLoading) {
		//FireLoop();
		//CL_UpdateAmmoUI(CurrentEquipWeapon);
		float ShootingSpeed = CurrentEquipWeapon->GetItemData()->ShootingSpeed;
		//GetWorld()->GetTimerManager().SetTimer(ShootWeaponTimerHandle, this, &ABattleCharacterBase::FireLoop, ShootingSpeed, true);
		bool AutoShoot;
		switch (CurrentEquipWeapon->GetItemData()->Type)
		{
		case EWeaponType::EWS_Rifle:
			AutoShoot = true;
			break;
		case EWeaponType::EWS_Pistol:
			AutoShoot = false;
			break;
		default:
			AutoShoot = true;
			break;
		}
		GetWorld()->GetTimerManager().SetTimer(ShootWeaponTimerHandle, [this]() {
			if (!bIsReLoading)
				FireLoop();
			}, ShootingSpeed, AutoShoot, 0);
	}
	else {
		GetWorld()->GetTimerManager().ClearTimer(ShootWeaponTimerHandle);
	}



}
void ABattleCharacterBase::ShootWeapon(const FInputActionValue& Value)
{
	bool bIsShooting = Value.Get<bool>();
	//FVector ViewLoc = ViewCamera->GetComponentLocation();
	//FVector ViewRot = ViewCamera->GetForwardVector();
	if (IsValid(CurrentEquipWeapon)) {
		if (CurrentEquipWeapon->GetItemData()->Type != EWeaponType::EWS_UnArm) {
			SRV_ShootWeapon(bIsShooting);
		}
		else {
			SRV_ShootWeapon(false);
		}
	}
}
void ABattleCharacterBase::SetIsReloadingWeapon(bool Value)
{
	SRV_SetIsReloadingWeapon(Value);
}
void ABattleCharacterBase::ReloadingWeapon()
{
	if (IsValid(CurrentEquipWeapon)) {
		if (EquippedWeaponType != EWeaponType::EWS_UnArm && !bIsReLoading && CurrentEquipWeapon->GetItemData()->MaxAmmoAmount != CurrentEquipWeapon->CurrentAmmo) {
			if (HasExtraAmmo()) {
				SetIsReloadingWeapon(true);
			}
		}
	}
}
void ABattleCharacterBase::SRV_SetIsReloadingWeapon_Implementation(bool Value)
{
	bIsReLoading = Value;
	if (bIsReLoading) {
		MC_SetIsReloadingWeapon(CurrentEquipWeapon->GetItemData()->ReloadAnimation);
	}
	else {
		if (CurrentEquipWeapon) {
			if (CurrentEquipWeapon->CurrentMaxExtraAmmo + CurrentEquipWeapon->CurrentAmmo >= CurrentEquipWeapon->GetItemData()->MaxAmmoAmount) {
				int32 CurrentAmoTemp = CurrentEquipWeapon->CurrentAmmo;
				CurrentEquipWeapon->CurrentAmmo = CurrentEquipWeapon->GetItemData()->MaxAmmoAmount;
				int32 CurremtMaxAmoTemp = CurrentEquipWeapon->CurrentMaxExtraAmmo - (CurrentEquipWeapon->GetItemData()->MaxAmmoAmount - CurrentAmoTemp);
				//CurrentEquipWeapon->CurrentMaxExtraAmmo -= CurrentEquipWeapon->GetItemData()->MaxAmmoAmount - CurrentAmoTemp;
				CurrentEquipWeapon->CurrentMaxExtraAmmo = UKismetMathLibrary::Clamp(CurremtMaxAmoTemp, 0, CurrentEquipWeapon->CurrentMaxExtraAmmo);
			}
			else {
				CurrentEquipWeapon->CurrentAmmo = UKismetMathLibrary::Clamp(CurrentEquipWeapon->CurrentMaxExtraAmmo + CurrentEquipWeapon->CurrentAmmo, 0, CurrentEquipWeapon->GetItemData()->MaxAmmoAmount);
				CurrentEquipWeapon->CurrentMaxExtraAmmo = 0;

			}
			CL_UpdateAmmoUI(CurrentEquipWeapon->CurrentAmmo, CurrentEquipWeapon->CurrentMaxExtraAmmo);
		}
	}
}
void ABattleCharacterBase::MC_SetIsReloadingWeapon_Implementation(UAnimMontage* ReloadAnim)
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	UAnimInstance* SkeletonAnim = SkeletonMesh->GetAnimInstance();
	Anim->Montage_Play(ReloadAnim);
	SkeletonAnim->Montage_Play(ReloadAnim);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUFunction(this, "CallSetIsReloadingWeaponFalse");
	SkeletonAnim->Montage_SetEndDelegate(EndDelegate, ReloadAnim);
}
void ABattleCharacterBase::CallSetIsReloadingWeaponFalse()
{

	if (HasAuthority()) {
		UE_LOG(LogTemp, Display, TEXT("CallSetIsReloadingWeaponFalse"));
		SetIsReloadingWeapon(false);
	}

}
void ABattleCharacterBase::MC_BulletImpact_Implementation(AWeaponBase* Weapon, FHitResult HitResult)
{
	if (HitResult.GetActor()->ActorHasTag("Player")) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->GetItemData()->BulletPlayerImpactVFX, HitResult.Location);
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), Weapon->GetItemData()->BulletPlayerImpactSFX, HitResult.Location);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Weapon->GetItemData()->BulletPlayerImpactSFX, HitResult.Location, 1.0f, 1.0f, 0.f, SoundAttenuation);
	}
	else {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->GetItemData()->BulletImpactVFX, HitResult.Location);
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), Weapon->GetItemData()->BulletImpactSFX, HitResult.Location);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Weapon->GetItemData()->BulletImpactSFX, HitResult.Location, 1.0f, 1.0f, 0.f, SoundAttenuation);
	}
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), Weapon->GetItemData()->BulletDecal, FVector(5, 5, 5), HitResult.Location, HitResult.ImpactNormal.Rotation(), 10);

}
float ABattleCharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//if (GEngine) {
	//	const FString FinalMSG = "Damage: " + FString::SanitizeFloat(Damage);
	//	GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::MakeRandomColor(), FinalMSG, true);
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMSG);
	//}
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	PlayerControllerInterface = EventInstigator;
	ABattleControllerBase* EnemyControllerBase = PlayerControllerInterface->GetPlayerControllerRef();
	if (EnemyControllerBase->TeamActor == PlayerController->TeamActor) {
		return Damage;
	}
	float TakeAmmorDmg = CurrentAmor - Damage * 1.5;
	SetPlayerAmor(UKismetMathLibrary::Clamp(TakeAmmorDmg, 0, MaxArmor));
	if (CurrentAmor <= 0) {
		SetPlayerHealth(UKismetMathLibrary::Clamp(CurrentHealth - Damage, 0, MaxHealth));
	}
	else {
		float HasAmmorDmg = Damage / 2.5;
		UE_LOG(LogTemp, Display, TEXT("Blocking: Less Dmage %f"), HasAmmorDmg);
		SetPlayerHealth(UKismetMathLibrary::Clamp(CurrentHealth - HasAmmorDmg, 0, MaxHealth));
	}
	if (CurrentHealth <= 0) {
		Death(true);
	}
	
	return Damage;
}
#pragma endregion
#pragma region Dead
void ABattleCharacterBase::SRV_DeathDropWeapon_Implementation()
{
	
	if (!WeaponInventory.IsEmpty()) {
		for (AWeaponBase* Weapon : WeaponInventory) {
			if (Weapon) {
				DropWeapon(Weapon);
			}
		}
	}
	FTimerHandle WeaponDropResetDelay;
	GetWorld()->GetTimerManager().SetTimer(WeaponDropResetDelay, [this]() {
		WeaponToDrop = nullptr;
		CurrentEquipWeapon = nullptr;
		OnRep_WeaponToDrop();
	}, 0.3, false);
}
void ABattleCharacterBase::SRV_Death_Implementation(bool bIsDeathSet)
{
	bIsDead = bIsDeathSet;
	OnRep_bIsDead();
}
void ABattleCharacterBase::Death(bool bIsDeathSet)
{
	SRV_Death(bIsDeathSet);
}
void ABattleCharacterBase::Respawn()
{
	SRV_Respawn();
}
void ABattleCharacterBase::OnRep_bIsDead()
{
	if (bIsDead) {
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		SkeletonMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
		//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletonMesh->SetSimulatePhysics(true);
		GetCharacterMovement()->DisableMovement();
		SRV_DeathDropWeapon();

		FTimerHandle RespawnTimer;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, [this]() {Respawn(); }, 5, false);
	}
	/*else {
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SkeletonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetSimulatePhysics(false);
		//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletonMesh->SetSimulatePhysics(false);
		GetCharacterMovement()->ResetMoveState();
	}*/
}
void ABattleCharacterBase::SRV_Respawn_Implementation()
{
	BattleGameModeInterface = GetWorld()->GetAuthGameMode();
	TObjectPtr<ABattleGameModeBase> GMBase = BattleGameModeInterface->GetBattleGameModeBase();
	GMBase->RequestSpawnPlayer(bIsBlueTeam, PlayerController);

	FTimerHandle DeleteDelay;
	GetWorld()->GetTimerManager().SetTimer(DeleteDelay,[this](){
		Destroy();
	},15.f,false);
}
void ABattleCharacterBase::ONnRep_PlayerSpawnTransform()
{
	SetActorTransform(PlayerSpawnTransform);
	Death(false);
}


#pragma endregion
#pragma region Movement
void ABattleCharacterBase::Look(const FInputActionValue& Value)
{
	const FVector2D LookVec = Value.Get<FVector2D>();
	if (Controller != nullptr) {
		AddControllerPitchInput(LookVec.Y);
		AddControllerYawInput(LookVec.X);
		if (HasAuthority()) {

		}
		else {
			SRV_CalculateAimDirection();
		}
		CalculateAimDirection();
	}

}
void ABattleCharacterBase::Move(const FInputActionValue& Value)
{
	FVector2D MoveVec = Value.Get<FVector2D>();
	if (Controller != nullptr) {
		AddMovementInput(GetActorForwardVector(), MoveVec.Y);
		AddMovementInput(GetActorRightVector(), MoveVec.X);
		//UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("%f"), UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity)));
		if (UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) > 100 && !GetCharacterMovement()->IsFalling()) {
			if (!bIsPlayingFootStepSFX || ChangeFootStepDelayTime != FootStepDelayTime) {
				bIsPlayingFootStepSFX = true;
				FootStepDelayTime = ChangeFootStepDelayTime;
				//UKismetSystemLibrary::PrintString(GetWorld(),FString("Change"));
				SRV_PlayFootStepSFX();
				GetWorld()->GetTimerManager().SetTimer(FootStepTimerHandle, [this]() {
					SRV_PlayFootStepSFX();
					}, FootStepDelayTime, 1);
			}
		}
		else {
			MoveComplete();
		}
	}
}
void ABattleCharacterBase::CustomCrouch(const FInputActionValue& Value)
{
	bool bIsCrouch = Value.Get<bool>();
	if (!GetCharacterMovement()->IsFalling() && !BattleMovementState.Contains(EMovementState::EMS_Running)) {
		if (!BattleMovementState.Contains(EMovementState::EMS_Crouching) && bIsCrouch) {
			if (HasAuthority()) {//?????????? ?????? ?κ?.

			}
			else {//???????.
				SRV_Crouch(bIsCrouch);
			}
			SetCrouch(bIsCrouch);
			UE_LOG(LogTemp, Display, TEXT("Crouch"));
		}
	}
}
void ABattleCharacterBase::SetBattleSprint(const FInputActionValue& Value)
{
	bool bIsSprint = Value.Get<bool>();
	//Crouch??? Sprint?? ??????? Sprint?? ?켱?????? ????????.
	if (BattleMovementState.Contains(EMovementState::EMS_Crouching)) {
		if (HasAuthority()) {//?????????? ?????? ?κ?.

		}
		else {//???????.
			SRV_Crouch(false);
		}
		SetCrouch(false);
	}
	if (HasAuthority()) {

	}
	else {
		SRV_BattleSprint(bIsSprint);
	}
	BattleSprint(bIsSprint);

}
void ABattleCharacterBase::CustomCrouchEnd(const FInputActionValue& Value)
{
	bool bIsCrouch = Value.Get<bool>();
	if (EMovementState::EMS_Crouching == BattleMovementState.Last()) {
		if (HasAuthority()) {

		}
		else {
			SRV_Crouch(bIsCrouch);
		}
		SetCrouch(bIsCrouch);
	}
}
void ABattleCharacterBase::SetCrouch(bool bIsCrouch)
{
	if (bIsCrouch) {
		BattleMovementState.Add(EMovementState::EMS_Crouching);
		bIsCrouching = true;
		Crouch();
	}
	else {
		BattleMovementState.Remove(EMovementState::EMS_Crouching);
		bIsCrouching = false;
		UnCrouch();
	}
	SetBattleCharacterSpeed();
}
void ABattleCharacterBase::BattleSprint(bool bIsSprint)
{
	if (bIsSprint) {
		BattleMovementState.Add(EMovementState::EMS_Running);
		bIsSprinting = true;
	}
	else {
		BattleMovementState.Remove(EMovementState::EMS_Running);
		bIsSprinting = false;
	}
	SetBattleCharacterSpeed();
}
void ABattleCharacterBase::MoveComplete()
{

	if (FootStepTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(FootStepTimerHandle);
		bIsPlayingFootStepSFX = false;
	}
}
void ABattleCharacterBase::SetBattleCharacterSpeed()
{
	//BattleMovementState.GetData()
	//if (BattleMovementState.Last() == EMovementState::EMS_Crouching) {
	//	Crouch();
	//}
	//else {
	//	UnCrouch();
	//}
	switch (BattleMovementState.Last())
	{
	case EMovementState::EMS_Walking:
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		ChangeFootStepDelayTime = 0.5;
		break;
	case EMovementState::EMS_Running:
		GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
		ChangeFootStepDelayTime = 0.42;
		break;
	case EMovementState::EMS_Crouching:
		GetCharacterMovement()->MaxWalkSpeedCrouched = MaxCrouchSpeed;
		break;
	default:
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		break;
	}
}
void ABattleCharacterBase::SRV_BattleSprint_Implementation(bool bIsSprint)
{
	BattleSprint(bIsSprint);
}
void ABattleCharacterBase::SRV_Crouch_Implementation(bool bIsCrouch)
{
	SetCrouch(bIsCrouch);
}
#pragma endregion
#pragma region MoveSFX
void ABattleCharacterBase::MC_PlayFootStepSFX_Implementation()
{
	if (SoundAttenuation && FootStepSoundArray[0]) {
		UGameplayStatics::PlaySoundAtLocation(this, FootStepSoundArray[0], GetActorLocation(),1.0f,1.0f,0.f, SoundAttenuation);
	}
}
void ABattleCharacterBase::SRV_PlayFootStepSFX_Implementation()
{
	MC_PlayFootStepSFX();
}
#pragma endregion
#pragma region Weapon
void ABattleCharacterBase::CL_UpdateInventoryUI_Implementation(EWeaponType WeaponType, UTexture2D* WeaponThumbnail)
{
	//PlayerController->BattleHUD->UpdateOverlayInventoryUI(WeaponType, WeaponThumbnail);
	OverlayWidget->UpdateOverlayInventoryUI(WeaponType, WeaponThumbnail);
}
void ABattleCharacterBase::MC_PlayEquipWeawponMontage_Implementation(UAnimMontage* WeaponMontage)
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	UAnimInstance* SkeletonAnim = SkeletonMesh->GetAnimInstance();
	Anim->Montage_Play(WeaponMontage);
	SkeletonAnim->Montage_Play(WeaponMontage);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUFunction(this, "CallSetIsEquippingFalse");
	SkeletonAnim->Montage_SetEndDelegate(EndDelegate, WeaponMontage);
}
void ABattleCharacterBase::SRV_SetIsEquipping_Implementation(bool bIsEquippingWeaponSet)
{
	bIsEquippingWeapon = bIsEquippingWeaponSet;
	if (bIsEquippingWeapon) {
		MC_PlayEquipWeawponMontage(CurrentEquipWeapon->GetItemData()->EquipAnimation);

	}
	else {
		bIsEquippingWeapon = false;
		if (IsOutOfAmmo() && HasExtraAmmo()) {
			SetIsReloadingWeapon(true);
		}
	}
}
void ABattleCharacterBase::SRV_EquipWeapon_Implementation(EWeaponType EWeaponEquippedType)
{
	for (TObjectPtr<AWeaponBase>Weapon_Eq : WeaponInventory) {
		if (Weapon_Eq->GetItemData()->Type == EWeaponEquippedType) {
			bIsEquipWeaponFound = true;
			if (EquippedWeaponType != EWeaponType::EWS_UnArm) {
				for (TObjectPtr<AWeaponBase>Weapon_Un : WeaponInventory) {
					if (Weapon_Un->GetItemData()->Type == EquippedWeaponType) {
						UnEquipWeapon = Weapon_Un;
						OnRep_UnEquipWeapon();
						break;
					}
				}
			}
			EquipWeapon = Weapon_Eq;
			OnRep_EquipWeapon();
			CurrentEquipWeapon = EquipWeapon;
			SetEquippedWeaponType(EWeaponEquippedType);
			CL_UpdateInventoryUI(EWeaponEquippedType, EquipWeapon->GetItemData()->Thumbnail);
			CL_UpdateAmmoUI(CurrentEquipWeapon->CurrentAmmo, CurrentEquipWeapon->CurrentMaxExtraAmmo);
			CL_DisplayAmmoUI(true);
			//PlayAnim
			SetIsEquipping(true);
			//MC_PlayEquipWeawponMontage(EquipWeapon->GetItemData()->EquipAnimation);
			EquipWeapon = nullptr;
			OnRep_EquipWeapon();
			UnEquipWeapon = nullptr;
			OnRep_UnEquipWeapon();
			bIsEquipWeaponFound = false;
			break;
		}
	}
	if (bIsEquipWeaponFound) {
		/*?ð????? ?????? ??????? RepNotify*/

	}
}
void ABattleCharacterBase::SetIsEquipping(bool bIsEquippingWeaponSet)
{
	SRV_SetIsEquipping(bIsEquippingWeaponSet);
}
void ABattleCharacterBase::CallSetIsEquippingFalse()
{
	UE_LOG(LogTemp, Display, TEXT("CallSetIsEquippingFalse"));
	SetIsEquipping(false);
}
void ABattleCharacterBase::SetEquippedWeaponType(EWeaponType WeaponType)
{
	SRV_SetEquippedWeaponType(WeaponType);
}
void ABattleCharacterBase::SRV_SetEquippedWeaponType_Implementation(EWeaponType WeaponType)
{
	EquippedWeaponType = WeaponType;
}
void ABattleCharacterBase::SRV_CalculateAimDirection_Implementation()
{
	CalculateAimDirection();
}
void ABattleCharacterBase::AddOverlapWeapon(AWeaponBase* Weapon)
{
	SRV_AddOverlapWeapon(Weapon);
}
void ABattleCharacterBase::SRV_AddOverlapWeapon_Implementation(AWeaponBase* Weapon)
{
	OverlapWeapons.Add(Weapon);

	if (bIsOverlapOnce) {
		bIsOverlapOnce = false;
		LootWeapons();
	}
}
void ABattleCharacterBase::CalculateAimDirection()
{
	FRotator Rot = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());
	AimDirection = Rot.Pitch;
}
void ABattleCharacterBase::LootWeapons()
{
	SRV_LootWeapons();
}
void ABattleCharacterBase::SRV_RequestLootWeapon_Implementation()
{
	if (!bLootingWeapon) {//???????? ???? ????. bLootingweapon ??????????? ??????? ??????? ??? ??? ??? ???????? ?????????
		if (HasAuthority()) {
			UKismetSystemLibrary::PrintString(GetWorld(), FString("SRV_RequestLootWeapon Excute"));
		}
		TArray<AActor*> Item;
		GetOverlappingActors(Item, AWeaponBase::StaticClass());
		if (!Item.IsEmpty()) {
			TObjectPtr<AWeaponBase>OverlapWeaponRef;
			if (Item[0]->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass())) {
				TScriptInterface<IWeaponInterface> WeaponInterface = Item[0];
				OverlapWeaponRef = WeaponInterface->GetWeaponRef();
			}
			for (TObjectPtr<AWeaponBase>Weapon : WeaponInventory) {
				if (Weapon->GetItemData()->Type == OverlapWeaponRef->GetItemData()->Type) {
					DropWeapon(Weapon);
					AddOverlapWeapon(OverlapWeaponRef);
					break;
					//UE_LOG(LogTemp, Display, TEXT("Weapon ex"));
				//	BattleCharacterRef->bCanLootWeapon = false;
				//	break;
				}

			}
			FTimerHandle WeaponDropResetDelay;
			GetWorld()->GetTimerManager().SetTimer(WeaponDropResetDelay, [this]() {
				if (WeaponToDrop) {
					//WeaponToDrop->bCanbeInteracted = true;
					WeaponToDrop = nullptr;
					OnRep_WeaponToDrop();
				}
				
			}, 0.1, false);

		}
	}
}
void ABattleCharacterBase::SRV_LootWeapons_Implementation()
{
	bLootingWeapon = true;
	if (!OverlapWeapons.IsEmpty()) {
		WeaponInventory.Add(OverlapWeapons[0]);
		if (LootWeaponBase == OverlapWeapons[0]) {
			LootWeaponBase = nullptr;
			OnRep_LootWeaponBase();
		}
		LootWeaponBase = OverlapWeapons[0];
		OnRep_LootWeaponBase();
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
			//UE_LOG(LogTemp, Display, TEXT("************************"));
			if (EquippedWeaponType == EWeaponType::EWS_UnArm) {
				CurrentEquipWeapon = LootWeaponBase;
				SetEquippedWeaponType(LootWeaponBase->GetItemData()->Type);
				SetIsEquipping(true);//MC_PlayEquipWeawponMontage(LootWeaponBase->GetItemData()->EquipAnimation);
				//CL_UpdateAmmoUI(CurrentEquipWeapon);
				CL_UpdateAmmoUI(CurrentEquipWeapon->CurrentAmmo, CurrentEquipWeapon->CurrentMaxExtraAmmo);
				CL_DisplayAmmoUI(true);
				CL_UpdateInventoryUI(LootWeaponBase->GetItemData()->Type, LootWeaponBase->GetItemData()->Thumbnail);

				LootWeaponBase = nullptr;
				OnRep_LootWeaponBase();
			}
			else {
				CL_UpdateInventoryUI(LootWeaponBase->GetItemData()->Type, LootWeaponBase->GetItemData()->Thumbnail);
			}
			OverlapWeapons.RemoveAt(0);
			LootWeapons();
			}, 0.05, false);
		//}
		//if (EquippedWeapon == EWeaponType::EWS_UnArm) {
		//	SetEquippedWeapon(LootWeaponBase->WeaponType);
		//	OverlapWeapons.Remove(LootWeaponBase);
		//	LootWeapons();
			//OverlapWeapons.Remove(LootWeaponBase);
		//}
		/*Check for OverlapWeapon*/
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("IsEmpty"));
		bIsOverlapOnce = true;
		FTimerHandle TempDelay;
		GetWorld()->GetTimerManager().SetTimer(TempDelay, [this]() {
			//LootWeaponBase = nullptr;
			//OnRep_LootWeaponBase();
			bLootingWeapon = false;
			}, 0.1, false);
	}

}
void ABattleCharacterBase::OnRep_EquipWeapon()
{
	if (IsValid(EquipWeapon)) {
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
		EquipWeapon->DetachFromActor(DetachmentTransformRules);

		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		FName WeaponSocketName;
		switch (EquipWeapon->GetItemData()->Type)
		{
		case EWeaponType::EWS_UnArm:
			WeaponSocketName = FName("None");
			break;
		case EWeaponType::EWS_Pistol:
			WeaponSocketName = FName("S_Pistol");
			break;
		case EWeaponType::EWS_Rifle:
			WeaponSocketName = FName("S_Rifle");
			break;
		default:
			break;
		}
		if (EquipWeapon->AttachToComponent(SkeletonMesh, AttachmentTransformRules, WeaponSocketName)) {
			//UE_LOG(LogTemp, Warning, TEXT("362 Success: %s"), *WeaponSocketName.ToString());
			if (HasAuthority()) {
				UKismetSystemLibrary::PrintString(GetWorld());
			}
			else {
				UKismetSystemLibrary::PrintString(GetWorld());
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("BattleCharacterBase.cpp AttachToComponentError"));
		}
	}

}
void ABattleCharacterBase::OnRep_UnEquipWeapon()
{
	if (IsValid(UnEquipWeapon)) {
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
		UnEquipWeapon->DetachFromActor(DetachmentTransformRules);
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		FName UnArmSocketName;
		switch (UnEquipWeapon->GetItemData()->Type)
		{
		case EWeaponType::EWS_UnArm:
			UnArmSocketName = FName("None");
			break;
		case EWeaponType::EWS_Pistol:
			UnArmSocketName = FName("S_Weapon_thigh");
			break;
		case EWeaponType::EWS_Rifle:
			UnArmSocketName = FName("S_Weapon_Back");
			break;
		default:
			break;
		}
		UnEquipWeapon->AttachToComponent(SkeletonMesh, AttachmentTransformRules, UnArmSocketName);
	}


}
void ABattleCharacterBase::OnRep_WeaponToDrop()
{
	if (IsValid(WeaponToDrop)) {
		UKismetSystemLibrary::PrintString(GetWorld(), "Drop Weapon");
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
		WeaponToDrop->DetachFromActor(DetachmentTransformRules);

		WeaponToDrop->SkeletalMeshComponent->SetSimulatePhysics(true);
		WeaponToDrop->SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		WeaponToDrop->Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}
void ABattleCharacterBase::OnRep_LootWeaponBase()
{
	if (IsValid(LootWeaponBase)) {
		LootWeaponBase->SkeletalMeshComponent->SetSimulatePhysics(false);
		LootWeaponBase->SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		LootWeaponBase->Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		if (EquippedWeaponType == EWeaponType::EWS_UnArm) {
			//UE_LOG(LogTemp, Warning, TEXT("WeaponSocketName"));
			FName WeaponSocketName;
			switch (LootWeaponBase->GetItemData()->Type)
			{
			case EWeaponType::EWS_UnArm:
				WeaponSocketName = FName("None");
				break;
			case EWeaponType::EWS_Pistol:
				WeaponSocketName = FName("S_Pistol");
				break;
			case EWeaponType::EWS_Rifle:
				WeaponSocketName = FName("S_Rifle");
				break;
			default:
				break;
			}
			if (LootWeaponBase->AttachToComponent(SkeletonMesh, AttachmentTransformRules, WeaponSocketName)) {
				//UE_LOG(LogTemp, Warning, TEXT("362 Success: %s"), *WeaponSocketName.ToString());
				if (HasAuthority()) {
					UKismetSystemLibrary::PrintString(GetWorld());
				}
				else {
					UKismetSystemLibrary::PrintString(GetWorld());
				}
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("BattleCharacterBase.cpp AttachToComponentError"));
			}
			//RepNotify?? ??? Server?? ???? ????????. ????? ??? SRV??????? ????.
			//SRV_SetEquippedWeapon(LootWeaponBase->WeaponType);
			//SetEquippedWeapon(LootWeaponBase->WeaponType);
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("UnArmSocketName"));
			FName UnArmSocketName;
			switch (LootWeaponBase->GetItemData()->Type)
			{
			case EWeaponType::EWS_UnArm:
				UnArmSocketName = FName("None");
				break;
			case EWeaponType::EWS_Pistol:
				UnArmSocketName = FName("S_Weapon_thigh");
				break;
			case EWeaponType::EWS_Rifle:
				UnArmSocketName = FName("S_Weapon_Back");
				break;
			default:
				break;
			}
			LootWeaponBase->AttachToComponent(SkeletonMesh, AttachmentTransformRules, UnArmSocketName);
		}
		// ????? ??????? ????
		LootWeaponBase->SetActorRelativeLocation(FVector::ZeroVector);
		// ????? ??????? ????
		LootWeaponBase->SetActorRelativeRotation(FRotator::ZeroRotator);
		// ??? ??????? ????
		LootWeaponBase->SetActorRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
}
void ABattleCharacterBase::DropWeapon(AWeaponBase* WeaponRef)
{
	WeaponRef->bCanbeInteracted = false;
	WeaponToDrop = WeaponRef;
	EWeaponType TmpType = WeaponToDrop->GetItemData()->Type;
	OnRep_WeaponToDrop();
	SetDroppedWeapon(WeaponToDrop);
	CL_DisplayAmmoUI(false);
	CL_UpdateInventoryUI(TmpType, nullptr);
	WeaponToDrop->bCanbeInteracted = true;
	WeaponInventory.Remove(WeaponToDrop);
	if (TmpType == EquippedWeaponType) {
		SetEquippedWeaponType(EWeaponType::EWS_UnArm);
		//SRV_SetEquippedWeaponType(EWeaponType::EWS_UnArm);
	}
}
void ABattleCharacterBase::SetDroppedWeapon(AWeaponBase* DroppedWeapon)
{
	SRV_SetDroppedWeapon(DroppedWeapon);
}
void ABattleCharacterBase::SRV_DropWeapon_Implementation()
{
	if (!WeaponInventory.IsEmpty() && EquippedWeaponType != EWeaponType::EWS_UnArm) {
		for (TObjectPtr<AWeaponBase> Weapon : WeaponInventory) {
			if (Weapon->GetItemData()->Type == EquippedWeaponType) {
				DropWeapon(Weapon);
				FTimerHandle WeaponDropResetDelay;
				GetWorld()->GetTimerManager().SetTimer(WeaponDropResetDelay, [this]() {
					UE_LOG(LogTemp, Display, TEXT("why...?"));
					//WeaponToDrop->bCanbeInteracted = true;
					WeaponToDrop = nullptr;
					CurrentEquipWeapon = nullptr;
					OnRep_WeaponToDrop();
					}, 0.1, false);
				break;
			}
		}
	}
}
void ABattleCharacterBase::SRV_SetDroppedWeapon_Implementation(AWeaponBase* DroppedWeapon)
{
	DroppedWeaponBase = DroppedWeapon;
	
}
/*Player Select Rifle*/
void ABattleCharacterBase::SelectPrimaryWeapon()
{
	//EquippedWeaponType?? Replicated?? ????.
	if (EquippedWeaponType != EWeaponType::EWS_Rifle) {
		SRV_EquipWeapon(EWeaponType::EWS_Rifle);
	}
}
/*Player Select Gun*/
void ABattleCharacterBase::SelectSecondaryWeapon()
{
	if (EquippedWeaponType != EWeaponType::EWS_Pistol) {
		SRV_EquipWeapon(EWeaponType::EWS_Pistol);
	}
}
#pragma endregion

/*Replicated Variable Setting*/ 
void ABattleCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//#include "Net/UnrealNetwork.h"
	//DOREPLIFETIME_CONDITION(ABattleCharacterBase, bIsCrouching);
	DOREPLIFETIME(ABattleCharacterBase, bIsCrouching);
	DOREPLIFETIME(ABattleCharacterBase, bIsSprinting);
	DOREPLIFETIME(ABattleCharacterBase, BattleMovementState);
	DOREPLIFETIME(ABattleCharacterBase, AimDirection);
	DOREPLIFETIME(ABattleCharacterBase, EquippedWeaponType);
	DOREPLIFETIME(ABattleCharacterBase, LootWeaponBase);
	DOREPLIFETIME(ABattleCharacterBase, WeaponToDrop);

	DOREPLIFETIME(ABattleCharacterBase, CurrentEquipWeapon);
	DOREPLIFETIME(ABattleCharacterBase, EquipWeapon);

	DOREPLIFETIME(ABattleCharacterBase, bIsEquippingWeapon);
	DOREPLIFETIME(ABattleCharacterBase, bIsReLoading);
	DOREPLIFETIME(ABattleCharacterBase, MaxHealth);
	DOREPLIFETIME(ABattleCharacterBase, CurrentHealth);
	DOREPLIFETIME(ABattleCharacterBase, MaxArmor);
	DOREPLIFETIME(ABattleCharacterBase, CurrentAmor);
	DOREPLIFETIME(ABattleCharacterBase, bIsDead);
}
/*Input*/
void ABattleCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABattleCharacterBase::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ABattleCharacterBase::MoveComplete);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABattleCharacterBase::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABattleCharacterBase::Jump);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ABattleCharacterBase::CustomCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ABattleCharacterBase::CustomCrouchEnd);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABattleCharacterBase::SetBattleSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABattleCharacterBase::SetBattleSprint);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &ABattleCharacterBase::SRV_DropWeapon);
		EnhancedInputComponent->BindAction(LootAction, ETriggerEvent::Started, this, &ABattleCharacterBase::SRV_RequestLootWeapon);
		EnhancedInputComponent->BindAction(SelectPrimaryWeaponAction, ETriggerEvent::Started, this, &ABattleCharacterBase::SelectPrimaryWeapon);
		EnhancedInputComponent->BindAction(SelectSecondaryWeaponAction, ETriggerEvent::Started, this, &ABattleCharacterBase::SelectSecondaryWeapon);
		EnhancedInputComponent->BindAction(LeftMouseAction, ETriggerEvent::Started, this, &ABattleCharacterBase::ShootWeapon);
		EnhancedInputComponent->BindAction(LeftMouseAction, ETriggerEvent::Completed, this, &ABattleCharacterBase::ShootWeapon);
		EnhancedInputComponent->BindAction(ReLoadAction, ETriggerEvent::Started, this, &ABattleCharacterBase::ReloadingWeapon);
		EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Started, this, &ABattleCharacterBase::SRV_TestFunc);
	}
}