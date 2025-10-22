#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthAppWidget.generated.h"

UCLASS()
class POC_API UHealthAppWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UButton* AddHealthButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RemoveHealthButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY()
	class APlayerCharacter* PlayerCharacterRef;

	UFUNCTION()
	void OnAddHealthClicked();

	UFUNCTION()
	void OnRemoveHealthClicked();

	UFUNCTION()
	void OnBackClicked();

	UFUNCTION()
	void UpdateHealthUI(float NewHealth);
};