#pragma once

#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"
namespace MultiplayDebug {
	static void Print(const FString& MSG, const FColor& InColor = FColor::MakeRandomColor(), int32 InKey = -1) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, InColor, MSG);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *MSG);
		}
	}
	static void DrawDebugBox(const UBoxComponent* InComponent, const bool SelectColor = false, const bool bShouldPersist = true) {
		if (!InComponent) {
			UE_LOG(LogTemp, Error, TEXT("DrawDebugBox called with null InComponent"));
			return;
		}
		FColor BoxColor = SelectColor ? FColor::Red : FColor::Green;
		DrawDebugBox(
			InComponent->GetWorld(),
			InComponent->GetComponentLocation(),  // �ڽ� �߽� ��ġ
			InComponent->GetScaledBoxExtent(),    // �ڽ� ũ�� (������)
			InComponent->GetComponentQuat(),      // �ڽ� ȸ��
			BoxColor,                             // ����
			bShouldPersist,                       // ���� ���� (false�� �����ϸ� �Ͻ������� ǥ��)
			1.0f                                  // ǥ�� ���� �ð�
		);
	}
}