// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BattleCharacterBaseAnimInstance.h"
#include "Character/BattleCharacterBase.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Kismet\KismetMathLibrary.h"

void UBattleCharacterBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//if (IsValid(TryGetPawnOwner())) {
	//	//if (BattleRef = Cast<ABattleCharacterBase>(TryGetPawnOwner())) {
	//	if (BattleRef = TryGetPawnOwner()) {
	//		BattleCharacterMovementRef = Cast<UCharacterMovementComponent>(BattleRef->GetMovementComponent());
	//	}
	//}
	if (IsValid(TryGetPawnOwner())) {
		if (BattleCharacterRef = Cast<ABattleCharacterBase>(TryGetPawnOwner())) {
		//if (BattleRef = TryGetPawnOwner()) {
			if (IsValid(BattleCharacterRef)) {
				BattleCharacterMovementRef = Cast<UCharacterMovementComponent>(BattleCharacterRef->GetMovementComponent());
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UBattleCharacterBaseAnimInstance: Fail to Get Pawn Owner"));
	}
}

void UBattleCharacterBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (BattleCharacterMovementRef) {
		BattleVelocity = BattleCharacterMovementRef->Velocity;
		MovementSpeed = UKismetMathLibrary::VSizeXY(BattleVelocity);
		MovementDir = CalculateDirection(BattleCharacterMovementRef->Velocity, BattleCharacterRef->GetActorRotation());
		bIsCrouch = BattleCharacterRef->GetbIsCrouch();
		bIsSprint = BattleCharacterRef->GetbIsSprint();
		bIsFalling = BattleCharacterMovementRef->IsFalling();
		BattleMovementState = BattleCharacterRef->GetEMovementState();
		AimDirection = BattleCharacterRef->GetAimDirection();
		EqippedWeapon = BattleCharacterRef->GetEquippedWeapon();
	}
}

float UBattleCharacterBaseAnimInstance::CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation)
{
	if (!Velocity.IsNearlyZero()) {//움직이고 있는경우.
		/* 로컬좌표계.
		* X축 캐릭터가 바라보는 방향.
		* Y축 캐릭터가 바라보는 방향에서 오른쪽.
		* Z축 캐릭터에서 하늘 방향.
		*/
		const FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		const FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		const FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		const FVector NormalizedVel = Velocity.GetSafeNormal2D();
		//DrawDebugLine(GetWorld(), BattleCharacterRef->GetActorLocation(), ForwardVector, FColor::Red, false,1, 0, 5.f);
		//DrawDebugLine(GetWorld(), BattleCharacterRef->GetActorLocation(), RightVector, FColor::Blue, false, 1, 0, 5.f);
		const float ForwardCosAngle = static_cast<float>(FVector::DotProduct(ForwardVector, NormalizedVel));
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));
		const float RightCosAngle = static_cast<float>(FVector::DotProduct(RightVector,NormalizedVel));
		if (RightCosAngle < 0.f) {
			ForwardDeltaDegree *= -1.f;
		}
		return ForwardDeltaDegree;

	}
	return 0.f;
}
