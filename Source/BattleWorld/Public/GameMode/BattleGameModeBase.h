// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/GameModeInterface.h"
#include "BattleGameModeBase.generated.h"

class APlayerStartBlueTeam;
class APlayerStartRedTeam;
class ABattleControllerBase;
/**
 * 
 */
UCLASS()
class BATTLEWORLD_API ABattleGameModeBase : public AGameModeBase, public IGameModeInterface
{
	GENERATED_BODY()
public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	void RequestSpawnPlayer(bool bIsBlueTeam, ABattleControllerBase* BattleController);
	TArray<TObjectPtr<AController>> PlayerControllerArray;
	//TArray<TObjectPtr<APlayerStartBlueTeam>> PlayerStartBlueTeam;
	//TArray<TObjectPtr<APlayerStartRedTeam>> PlayerStartRedTeam;
	TArray<AActor*> BlueTeam;
	TArray<AActor*> RedTeam;


	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AActor> BluePlayer;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AActor> RedPlayer;
	//TSubclassOf<AActor> ComparePlayer;
	// IGameModeInterface을(를) 통해 상속됨
	virtual ABattleGameModeBase* GetBattleGameModeBase() override;
	//UPROPERTY(EditAnywhere)
	//TSubclassOf<AActor> Teams;
};
