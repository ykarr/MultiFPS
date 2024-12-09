// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverHeadWidgetBase.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UOverHeadWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetDisplayText(FString TextToDisplay);
	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);
protected:
	virtual void NativeConstruct()override;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DisplayText;


	virtual void NativeDestruct() override;
};
