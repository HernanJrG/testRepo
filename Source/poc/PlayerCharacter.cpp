#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "NoteData.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	bIsPhoneOpen = false;
	bIsNotesAppOpen = false;
	bIsHealthAppOpen = false;

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (PhoneWidgetClass)
	{
		PhoneWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), PhoneWidgetClass);
		if (PhoneWidgetInstance)
		{
			PhoneWidgetInstance->AddToViewport();
			PhoneWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (NotesAppWidgetClass)
	{
		NotesAppWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), NotesAppWidgetClass);
		if (NotesAppWidgetInstance)
		{
			NotesAppWidgetInstance->AddToViewport(1);
			NotesAppWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (HealthAppWidgetClass)
	{
		HealthAppWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HealthAppWidgetClass);
		if (HealthAppWidgetInstance)
		{
			HealthAppWidgetInstance->AddToViewport(1);
			HealthAppWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("TogglePhone", IE_Pressed, this, &APlayerCharacter::TogglePhone);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("TurnCamera", this, &APlayerCharacter::LookUp);
}

void APlayerCharacter::MoveForward(float value)
{
	if (bIsPhoneOpen) return;
	
	FVector ForwardDirection = GetActorForwardVector();
	AddMovementInput(ForwardDirection, value);
}

void APlayerCharacter::MoveRight(float value)
{
	if (bIsPhoneOpen) return;
	
	FVector RightDirection = GetActorRightVector();
	AddMovementInput(RightDirection, value);
}

void APlayerCharacter::Turn(float value)
{
	if (bIsPhoneOpen) return;
	
	AddControllerYawInput(value);
}

void APlayerCharacter::LookUp(float value)
{
	if (bIsPhoneOpen) return;
	
	AddControllerPitchInput(value);
}

void APlayerCharacter::TogglePhone()
{
	if (!PhoneWidgetInstance) return;

	bIsPhoneOpen = !bIsPhoneOpen;

	if (bIsPhoneOpen)
	{

		PhoneWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->bShowMouseCursor = true;
			PC->bEnableClickEvents = true;
			PC->bEnableMouseOverEvents = true;
		}
	}
	else
	{

		PhoneWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		
		if (bIsNotesAppOpen)
		{
			CloseNotesApp();
		}
		
		if (bIsHealthAppOpen)
		{
			CloseHealthApp();
		}
		
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->bShowMouseCursor = false;
			PC->bEnableClickEvents = false;
			PC->bEnableMouseOverEvents = false;
		}
	}
}

void APlayerCharacter::OpenNotesApp()
{
	if (!NotesAppWidgetInstance) return;

	bIsNotesAppOpen = true;
	NotesAppWidgetInstance->SetVisibility(ESlateVisibility::Visible);
}

void APlayerCharacter::CloseNotesApp()
{
	if (!NotesAppWidgetInstance) return;

	bIsNotesAppOpen = false;
	NotesAppWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
}

void APlayerCharacter::CreateNote(FString Title, FString Content)
{
	FNoteData NewNote(Title, Content);
	SavedNotes.Add(NewNote);
}

void APlayerCharacter::UpdateNote(int32 Index, FString Title, FString Content)
{
	if (SavedNotes.IsValidIndex(Index))
	{
		SavedNotes[Index].Title = Title;
		SavedNotes[Index].Content = Content;
	}
}

void APlayerCharacter::DeleteNote(int32 Index)
{
	if (SavedNotes.IsValidIndex(Index))
	{
		SavedNotes.RemoveAt(Index);
	}
}

void APlayerCharacter::OpenHealthApp()
{
	if (!HealthAppWidgetInstance) return;

	bIsHealthAppOpen = true;
	HealthAppWidgetInstance->SetVisibility(ESlateVisibility::Visible);
}

void APlayerCharacter::CloseHealthApp()
{
	if (!HealthAppWidgetInstance) return;

	bIsHealthAppOpen = false;
	HealthAppWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
}

void APlayerCharacter::AddHealth(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health added! Current Health: %f"), CurrentHealth);
	
	OnHealthChanged.Broadcast(CurrentHealth);
}

void APlayerCharacter::RemoveHealth(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health removed! Current Health: %f"), CurrentHealth);
	
	OnHealthChanged.Broadcast(CurrentHealth);
}

float APlayerCharacter::GetCurrentHealth() const
{
	return CurrentHealth;
}

float APlayerCharacter::GetHealthPercentage() const
{
	return (MaxHealth > 0.0f) ? (CurrentHealth / MaxHealth) : 0.0f;
}