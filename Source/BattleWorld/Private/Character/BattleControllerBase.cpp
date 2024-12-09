// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BattleControllerBase.h"
#include "GameMode/BattleGameModeBase.h"
#include "HUD/BattleHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Camera\CameraActor.h"
#include "Interface/HUDInterface.h"
#include "Engine\AssetManager.h"
void ABattleControllerBase::BeginPlay()
{
	if (HasAuthority()) {
		BattleGameModeInterface = GetWorld()->GetAuthGameMode();
		BattleGameModeBase = BattleGameModeInterface->GetBattleGameModeBase();
	}
	BattleHUD = Cast<ABattleHUD>(GetHUD());
	AActor* CameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
	SetViewTargetWithBlend(CameraActor);

}

/*GameMode에서 실행하므로 다른설정필요없음. 어차피 GameMode는 서버에서만 실행됨.*/
void ABattleControllerBase::SpawnPlayer(TSoftClassPtr<AActor> Team, FTransform SPawnPlayerLoc)
{
	FActorSpawnParameters ActorSpawnParameters;
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(Team.ToSoftObjectPath(),FStreamableDelegate::CreateLambda([this, Team, SPawnPlayerLoc, ActorSpawnParameters](){
		if (TeamActor=Team.Get()) {
			AActor* PlayerPawn = GetWorld()->SpawnActor<AActor>(Team.Get(), SPawnPlayerLoc, ActorSpawnParameters);
			Possess(Cast<APawn>(PlayerPawn));
			CL_SpawnPlayer();
		}
	}));
}

void ABattleControllerBase::CL_SpawnPlayer_Implementation()
{
	if (BattleHUD) {
		BattleHUD->ShowHideWidget(EGameWidget::EGW_TeamSelection, false);
		//BattleHUD->ShowHideWidget(EGameWidget::EGW_OverlayWidget, true);
		BattleHUD->ShowHideWidget(EGameWidget::EGW_CrossHairWidget, true);
		UE_LOG(LogTemp, Display, TEXT("CL_SpawnPlayer_Implementation_error"));
	}
	
	//BattleHUD->InitProgressBind();

	//FInputModeGameOnly InputMode;
	//this->SetInputMode(InputMode);
	//this->bShowMouseCursor = false;

}

void ABattleControllerBase::SelectTeam(bool bIsBlueTeam)
{
	SRV_SelectTeam(bIsBlueTeam);
}

void ABattleControllerBase::SRV_SelectTeam_Implementation(bool bIsBlueTeam)
{
	//TeamActor = Team;
	BattleGameModeBase->RequestSpawnPlayer(bIsBlueTeam,this);
}


ABattleControllerBase* ABattleControllerBase::GetPlayerControllerRef()
{
	return this;
}
