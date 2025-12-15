// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestTypes.h"
#include "QuestDefinition.generated.h"

UCLASS(BlueprintType)
class SIMPLEQUESTSYSTEM_API UQuestDefinition : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FQuestObjective> Objectives;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Quest")
	bool bIsRepeatable = false;

	/** Cooldown in seconds (0 = no cooldown). Used only if repeatable. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Quest")
	float RepeatCooldownSeconds = 0.0f;

	/** Optional prerequisites (QuestIDs that must be completed at least once). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Quest")
	TArray<FName> RequiredCompletedQuests;
};
