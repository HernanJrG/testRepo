#include "HealthAppWidget.h"
#include "PlayerCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UHealthAppWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PlayerCharacterRef = Cast<APlayerCharacter>(PlayerPawn);

	if (PlayerCharacterRef)
	{
		PlayerCharacterRef->OnHealthChanged.AddDynamic(this, &UHealthAppWidget::UpdateHealthUI);

		UpdateHealthUI(PlayerCharacterRef->GetCurrentHealth());
	}

	if (AddHealthButton)
	{
		AddHealthButton->OnClicked.AddDynamic(this, &UHealthAppWidget::OnAddHealthClicked);
	}

	if (RemoveHealthButton)
	{
		RemoveHealthButton->OnClicked.AddDynamic(this, &UHealthAppWidget::OnRemoveHealthClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UHealthAppWidget::OnBackClicked);
	}
}

void UHealthAppWidget::NativeDestruct()
{
	if (PlayerCharacterRef && PlayerCharacterRef->OnHealthChanged.IsBound())
	{
		PlayerCharacterRef->OnHealthChanged.RemoveDynamic(this, &UHealthAppWidget::UpdateHealthUI);
	}

	Super::NativeDestruct();
}

void UHealthAppWidget::UpdateHealthUI(float NewHealth)
{
	if (!PlayerCharacterRef) return;

	if (HealthBar)
	{
		float Percentage = PlayerCharacterRef->GetHealthPercentage();
		HealthBar->SetPercent(Percentage);
	}

	if (HealthText)
	{
		FString HealthString = FString::Printf(TEXT("%.0f / %.0f"), NewHealth, PlayerCharacterRef->MaxHealth);
		HealthText->SetText(FText::FromString(HealthString));
	}
}

void UHealthAppWidget::OnAddHealthClicked()
{
	if (PlayerCharacterRef)
	{
		PlayerCharacterRef->AddHealth(10.0f);
	}
}

void UHealthAppWidget::OnRemoveHealthClicked()
{
	if (PlayerCharacterRef)
	{
		PlayerCharacterRef->RemoveHealth(10.0f);
	}
}

void UHealthAppWidget::OnBackClicked()
{
	if (PlayerCharacterRef)
	{
		PlayerCharacterRef->CloseHealthApp();
	}
}