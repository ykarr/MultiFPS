// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BattleGameModeBase.h"
#include "Kismet\GameplayStatics.h"
//#include "Character/PlayerStartBase.h"
#include "Character/PlayerStartBlueTeam.h"
#include "Character/PlayerStartRedTeam.h"
#include "Character/BattleControllerBase.h"
#include "Character/BattleCharacterBlue.h"
void ABattleGameModeBase::OnPostLogin(AController* NewPlayer)
{
	PlayerControllerArray.AddUnique(NewPlayer);
	UE_LOG(LogTemp, Display, TEXT("start"));
	//UGameplayStatics::GetAllActorOfClass(GetWorld(), APlayerStartBlueTeam::StaticClass(), PlayerStartBlueTeam);
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStartBase::StaticClass(),);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStartBlueTeam::StaticClass(), BlueTeam);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStartRedTeam::StaticClass(), RedTeam);
}

void ABattleGameModeBase::RequestSpawnPlayer(bool bIsBlueTeam, ABattleControllerBase* BattleController)
{
	if (bIsBlueTeam) {
		const int32 Index = FMath::RandRange(0, BlueTeam.Num() - 1);
		FTransform transform = BlueTeam[Index]->GetActorTransform();
		BattleController->SpawnPlayer(BluePlayer, transform);
		//UE_LOG(LogTemp, Warning, TEXT("Blue"));
	}
	else {
		const int32 Index = FMath::RandRange(0, RedTeam.Num() - 1);
		FTransform transform = RedTeam[Index]->GetActorTransform();
		BattleController->SpawnPlayer(RedPlayer, transform);
		//UE_LOG(LogTemp, Warning, TEXT("Red"));
	}
	
	
	//BattleController->SpawnPlayer(Team,);
	//BlueTeam.
}

ABattleGameModeBase* ABattleGameModeBase::GetBattleGameModeBase()
{
	return this;
}
