// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameModeInterface.generated.h"
class ABattleGameModeBase;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BATTLEWORLD_API IGameModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual ABattleGameModeBase* GetBattleGameModeBase() = 0;
};
