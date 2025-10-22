#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NoteData.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);

UCLASS()
class POC_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> PhoneWidgetClass;

	UPROPERTY()
	class UUserWidget* PhoneWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> NotesAppWidgetClass;

	UPROPERTY()
	class UUserWidget* NotesAppWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HealthAppWidgetClass;

	UPROPERTY()
	class UUserWidget* HealthAppWidgetInstance;
	
	bool bIsPhoneOpen;
	bool bIsNotesAppOpen;
	bool bIsHealthAppOpen;
	
	UPROPERTY(BlueprintReadWrite, Category = "Notes")
	TArray<FNoteData> SavedNotes;
	
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void Turn(float Value);
	void LookUp(float Value);
	
	void TogglePhone();
	
	UFUNCTION(BlueprintCallable, Category = "Notes")
	void OpenNotesApp();

	UFUNCTION(BlueprintCallable, Category = "Notes")
	void CloseNotesApp();

	UFUNCTION(BlueprintCallable, Category = "Notes")
	void CreateNote(FString Title, FString Content);

	UFUNCTION(BlueprintCallable, Category = "Notes")
	void UpdateNote(int32 Index, FString Title, FString Content);

	UFUNCTION(BlueprintCallable, Category = "Notes")
	void DeleteNote(int32 Index);
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void OpenHealthApp();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void CloseHealthApp();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void RemoveHealth(float Amount);

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercentage() const;
	
};