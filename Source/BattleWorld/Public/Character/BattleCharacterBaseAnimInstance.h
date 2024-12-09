// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/CharacterType.h"
#include "BattleCharacterBaseAnimInstance.generated.h"

class UCharacterMovementComponent;
class ABattleCharacterBase;
/**
 * 
 */
UCLASS()
class BATTLEWORLD_API UBattleCharacterBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;
	float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);
	
	TObjectPtr<UCharacterMovementComponent> BattleCharacterMovementRef;
	TObjectPtr<ABattleCharacterBase> BattleCharacterRef;
	TObjectPtr<APawn> BattleRef;

	UPROPERTY(BlueprintReadOnly)
	float AimDirection;
	UPROPERTY(BlueprintReadOnly)
	float MovementSpeed;
	UPROPERTY(BlueprintReadOnly)
	float MovementDir;
	UPROPERTY(BlueprintReadOnly)
	bool bIsCrouch;
	UPROPERTY(BlueprintReadOnly)
	bool bIsSprint;
	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling;
	UPROPERTY(BlueprintReadOnly)
	FVector BattleVelocity;
	UPROPERTY(BlueprintReadOnly)
	TArray<EMovementState> BattleMovementState;
	UPROPERTY(BlueprintReadOnly)
	EWeaponType EqippedWeapon;
};
