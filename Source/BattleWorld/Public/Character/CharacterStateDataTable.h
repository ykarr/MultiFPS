// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStateDataTable.generated.h"

USTRUCT()
struct FStateData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
	float MaxHealth;
	UPROPERTY(EditAnywhere)
	float MaxArmor;
	UPROPERTY(EditAnywhere)
	float RunSpeed;
	UPROPERTY(EditAnywhere)
	float WalkSpeed;
	UPROPERTY(EditAnywhere)
	float CrouchSpeed;
};
/**
 * 
 */
UCLASS()
class BATTLEWORLD_API UCharacterStateDataTable : public UDataTable
{
	GENERATED_BODY()
public:
};
