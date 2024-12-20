#pragma once
UENUM(BluePrintType)
enum class EMovementState :uint8 {
	EMS_Walking UMETA(DisplayName = "Walking"),
	EMS_Running UMETA(DisplayName = "Runing"),
	EMS_Crouching UMETA(DisplayName = "Crouching")
};

UENUM(BluePrintType)
enum class EWeaponType :uint8 {
	EWS_UnArm UMETA(DisplayName = "UnArm"),
	EWS_Pistol UMETA(DisplayName = "Pistol"),
	EWS_Rifle UMETA(DisplayName = "Rifle")
};