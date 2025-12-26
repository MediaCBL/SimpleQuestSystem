// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "QuestTypes.h"
#include "QuestDefinition.h"
#include "QuestInstance.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdated, UQuestInstance*, Quest);

UCLASS(BlueprintType)
class SIMPLEQUESTSYSTEM_API UQuestInstance : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	const UQuestDefinition* Definition;

	UPROPERTY(BlueprintReadOnly)
	TArray<FObjectiveState> ObjectiveStates;

	UPROPERTY(BlueprintReadOnly)
	bool bCompleted = false;
	/** Optional per-quest flags for extensions. */
	UPROPERTY(BlueprintReadOnly)
	TMap<FName, bool> QuestFlags;
	
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentStep = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly)
	EQuestState QuestState;

	//UPROPERTY(BlueprintAssignable)
	//FOnQuestUpdated OnQuestUpdated;

	void Init(const UQuestDefinition* Def);

	bool TryProgress(int32 ObjectiveIndex, int32 Amount = 1);
	
	void RestoreFromSaveData(const FQuestSaveData& SaveData);
	bool IsCompleted() const { return bCompleted; }
	int32 GetCurrentStep() const { return CurrentStep; }
	void ExportToSaveData(FQuestSaveData& OutSaveData) const;

private:
	void CheckCompletion();
};

