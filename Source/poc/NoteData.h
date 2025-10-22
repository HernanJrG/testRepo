#pragma once

#include "CoreMinimal.h"
#include "NoteData.generated.h"

USTRUCT(BlueprintType)
struct FNoteData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Title;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Content;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDateTime DateCreated;

	FNoteData()
	{
		Title = TEXT("New Note");
		Content = TEXT("");
		DateCreated = FDateTime::Now();
	}

	FNoteData(FString InTitle, FString InContent)
	{
		Title = InTitle;
		Content = InContent;
		DateCreated = FDateTime::Now();
	}
};