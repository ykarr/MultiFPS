// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerControllerInterface.generated.h"

class ABattleControllerBase;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BATTLEWORLD_API IPlayerControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual ABattleControllerBase* GetPlayerControllerRef()=0;
};
