// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Character/BattleCharacterBase.h"
#include <Net\UnrealNetwork.h>
#include "Items/Data/ItemDataTable.h"



// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());
	SkeletalMeshComponent->SetRelativeRotation(FRotator(0,-90,0));
	SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//SkeletalMeshComponent->SetUseCCD(true);
	SetRootComponent(SkeletalMeshComponent);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetSphereRadius(75.f);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::SphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::SphereEndOverlap);
	SkeletalMeshComponent->SetSimulatePhysics(true);
	bReplicates = true;
	SetReplicateMovement(true);
	bCanbeInteracted = true;
	//Debug
	//Sphere->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle StartDelay;
	if (HasAuthority()) {
		SRV_InitWeapon();
	}
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


FItemData* AWeaponBase::GetItemData()
{
	if (WeaponInfo) return WeaponInfo;
	if (!WeaponName.IsEmpty()) {
		WeaponInfo = WeaponDataTable->FindRow<FItemData>(FName(WeaponName), WeaponName);
		return WeaponInfo;
	}
	return nullptr;
}

void AWeaponBase::BeginOverlapAction(AActor* OtherActor)
{
	TArray<AActor*> Actors;
	GetWorld()->GetTimerManager().ClearTimer(OverlapDelay);
	GetOverlappingActors(Actors,ABattleCharacterBase::StaticClass());
	if (Actors.Num() > 1) {
		UE_LOG(LogTemp, Display, TEXT("Multiple_Player"));
		for (TObjectPtr<AActor> PlayerActor : Actors) {
			TObjectPtr<ABattleCharacterBase> PlayerBase = Cast<ABattleCharacterBase>(PlayerActor);
			if (PlayerBase->DroppedWeaponBase != this) {
				BattleCharacterRef = PlayerBase;
				break;
			}
		}
		for (TObjectPtr<AWeaponBase>Weapon : BattleCharacterRef->WeaponInventory) {
			if (Weapon->GetItemData()->Type == GetItemData()->Type) {
				BattleCharacterRef->bCanLootWeapon = false;
				break;
			}
		}
		if (BattleCharacterRef->bCanLootWeapon) {
			BattleCharacterRef->AddOverlapWeapon(this);
		}
		else {
			BattleCharacterRef->bCanLootWeapon = true;
		}
	}
	else if(!Actors.IsEmpty()){
		BattleCharacterRef = Cast<ABattleCharacterBase>(Actors[0]);
		if (BattleCharacterRef->DroppedWeaponBase != this) {
			TArray<TObjectPtr<AWeaponBase>> Weapons = BattleCharacterRef->WeaponInventory;
			for (TObjectPtr<AWeaponBase>Weapon : BattleCharacterRef->WeaponInventory) {
				if (Weapon->GetItemData()->Type == GetItemData()->Type) {
					BattleCharacterRef->bCanLootWeapon = false;
					break;
				}
			}
			if (BattleCharacterRef->bCanLootWeapon) {
				//BattleCharacterRef->LootWeapons(this);
				//BattleCharacterRef->bIsOverlapOnce = true;
				BattleCharacterRef->AddOverlapWeapon(this);
			}
			else {
				BattleCharacterRef->bCanLootWeapon = true;
			}
		}
	}
	
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AWeaponBase, WeaponType);
	DOREPLIFETIME(AWeaponBase, CurrentAmmo);
	DOREPLIFETIME(AWeaponBase, CurrentMaxExtraAmmo);
}
void AWeaponBase::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor &&OtherActor->ActorHasTag("Player")) {

		if (HasAuthority()) {
			//<ABattleCharacterBase> BattleCharacterRef = Cast<ABattleCharacterBase>(OtherActor);
			//떨어뜨린 무기인지 확인해서 떨어뜨린 무기라면 Sphere의 밖으로 나갔다 와야 집을 수 있도록.
			
			if (bCanbeInteracted) {

				BeginOverlapAction(OtherActor);
			}
			
		}
	}
}
void AWeaponBase::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Player")) {

		if (HasAuthority()) {
			TObjectPtr<ABattleCharacterBase> PlayerBase = Cast<ABattleCharacterBase>(OtherActor);
			PlayerBase->SetDroppedWeapon(nullptr);
		}
	}
}

void AWeaponBase::TempFunc()
{
	
}

void AWeaponBase::SRV_InitWeapon_Implementation()
{
	CurrentAmmo= GetItemData()->MaxAmmoAmount;
	CurrentMaxExtraAmmo= GetItemData()->MaxExtraAmmo;

	
	
}

AWeaponBase* AWeaponBase::GetWeaponRef()
{
	return this;
}
