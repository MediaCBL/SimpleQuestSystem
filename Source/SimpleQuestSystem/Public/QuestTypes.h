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

/**
 * Quest runtime state (what the player is currently doing / has done).
 */
UENUM(BlueprintType)
enum class EQuestState : uint8
{
	Inactive	UMETA(DisplayName="Inactive"),
	Active		UMETA(DisplayName="Active"),
	Completed	UMETA(DisplayName="Completed"),
	Failed		UMETA(DisplayName="Failed"),
};

/**
 * Quest availability result for offer/give checks (for dialog/UI branching).
 */
UENUM(BlueprintType)
enum class EQuestAvailability : uint8
{
	Available					UMETA(DisplayName="Available"),
	AlreadyActive				UMETA(DisplayName="Already Active"),
	AlreadyCompleted			UMETA(DisplayName="Already Completed"),
	OnCooldown					UMETA(DisplayName="On Cooldown"),
	LockedByPrerequisite		UMETA(DisplayName="Locked By Prerequisite"),
	AlreadyGivenByThisGiver		UMETA(DisplayName="Already Given By This Giver"),
	InvalidQuest				UMETA(DisplayName="Invalid Quest"),
};

/**
 * Quest giver per-quest interaction state.
 */
UENUM(BlueprintType)
enum class EQuestGiverQuestState : uint8
{
	None	UMETA(DisplayName="None"),
	Given	UMETA(DisplayName="Given"),
	// Optional future:
	// Completed UMETA(DisplayName="Completed"),
	// Locked    UMETA(DisplayName="Locked"),
};

/**
 * Save-state per quest (no UObject refs).
 */
USTRUCT(BlueprintType)
struct SIMPLEQUESTSYSTEM_API FQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestState State = EQuestState::Inactive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStep = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TimesCompleted = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime LastCompletedTime;

	/** Optional per-quest flags for extensions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, bool> QuestFlags;
};

/**
 * Save-state per quest giver (no UObject refs).
 */
USTRUCT(BlueprintType)
struct SIMPLEQUESTSYSTEM_API FQuestGiverSaveData
{
	GENERATED_BODY()

	/** Per quest state for this giver */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, EQuestGiverQuestState> QuestStates;
};
