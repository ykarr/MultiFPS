// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/PlayerControllerInterface.h"
#include "BattleControllerBase.generated.h"

class ABattleHUD;
class ABattleGameModeBase;
class IHUDInterface;
class IGameModeInterface;
/**
 * 
 */
UCLASS()
class BATTLEWORLD_API ABattleControllerBase : public APlayerController,public IPlayerControllerInterface
{
	GENERATED_BODY()
public: 
	virtual void BeginPlay() override;
	void SpawnPlayer(TSoftClassPtr<AActor>Team,FTransform SPawnPlayerLoc );
	UFUNCTION(Client, Reliable)
	void CL_SpawnPlayer();
	void SelectTeam(bool bIsBlueTeam);
	UFUNCTION(server, Reliable)
	void SRV_SelectTeam(bool bIsBlueTeam);
	// IPlayerControllerInterface을(를) 통해 상속됨
	ABattleControllerBase* GetPlayerControllerRef() override;

	TObjectPtr< ABattleGameModeBase> BattleGameModeBase;
	//TObjectPtr<ABattleHUD> BattleHUD;
	ABattleHUD* BattleHUD;

	TScriptInterface<IHUDInterface> BattleHUDInterface;
	TScriptInterface<IGameModeInterface> BattleGameModeInterface;
	//When Attack Check team
	TSubclassOf<AActor> TeamActor;
	//TSoftClassPtr<AActor> TeamActor;
};
