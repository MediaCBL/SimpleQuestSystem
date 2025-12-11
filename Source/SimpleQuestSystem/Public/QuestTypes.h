#pragma once

#include "CoreMinimal.h"
#include "QuestTypes.generated.h"

UENUM(BlueprintType)
enum class EQObjectiveType : uint8
{
	Kill  UMETA(DisplayName="Kill"),
	Collect UMETA(DisplayName="Collect"),
	ReachLocation UMETA(DisplayName="Reach Location"),
	Custom UMETA(DisplayName="Custom")
};

USTRUCT(BlueprintType)
struct FQuestObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQObjectiveType Type = EQObjectiveType::Kill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TargetCount = 1;
};

USTRUCT(BlueprintType)
struct FObjectiveState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentCount = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bCompleted = false;
};
