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
			InComponent->GetComponentLocation(),  // 박스 중심 위치
			InComponent->GetScaledBoxExtent(),    // 박스 크기 (반지름)
			InComponent->GetComponentQuat(),      // 박스 회전
			BoxColor,                             // 색상
			bShouldPersist,                       // 영구 여부 (false로 설정하면 일시적으로 표시)
			1.0f                                  // 표시 지속 시간
		);
	}
}