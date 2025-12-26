// © Felix Courteau-Boutin All Right Reserved

#include "QuestInstance.h"

void UQuestInstance::Init(const UQuestDefinition* Def)
{
	Definition = Def;
	ObjectiveStates.SetNum(Definition->Objectives.Num());
	CurrentStep = 0;
	QuestState = EQuestState::Active;
}
void UQuestInstance::RestoreFromSaveData(const FQuestSaveData& SaveData)
{
	if (!Definition)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("RestoreFromSaveData called without Definition"));
		return;
	}

	const int32 NumObjectives = Definition->Objectives.Num();
	ObjectiveStates.SetNum(NumObjectives);

	// Reset all objectives
	for (int32 i = 0; i < NumObjectives; ++i)
	{
		FObjectiveState& ObjState = ObjectiveStates[i];
		const FQuestObjective& ObjDef = Definition->Objectives[i];

		const int32 SavedCount =
			SaveData.ObjectiveCounts.IsValidIndex(i)
			? SaveData.ObjectiveCounts[i]
			: 0;

		ObjState.CurrentCount = FMath::Clamp(SavedCount, 0, ObjDef.TargetCount);
		ObjState.bCompleted = (ObjState.CurrentCount >= ObjDef.TargetCount);
	}

	// Restore based on quest state
	QuestState = SaveData.State;
	switch (QuestState)
	{
	case EQuestState::Inactive:
		bCompleted = false;
		break;

	case EQuestState::Active:
		{
			CurrentStep = SaveData.CurrentStep;

			// Mark previous steps as completed
			for (int32 i = 0; i < NumObjectives; ++i)
			{
				if (i < CurrentStep)
				{
					ObjectiveStates[i].bCompleted = true;
				}
			}

			bCompleted = false;
		}
		break;

	case EQuestState::Completed:
		{
			for (FObjectiveState& ObjState : ObjectiveStates)
			{
				ObjState.bCompleted = true;
			}

			bCompleted = true;
		}
		break;

	case EQuestState::Failed:
		bCompleted = false;
		break;
	}

	// Optional: cache extension data
	QuestFlags = SaveData.QuestFlags;
}

bool UQuestInstance::TryProgress(int32 ObjectiveIndex, int32 Amount)
{
	if (bCompleted) return false;

	FObjectiveState& State = ObjectiveStates[ObjectiveIndex];
	const FQuestObjective& Obj = Definition->Objectives[ObjectiveIndex];

	if (State.bCompleted) return false;

	State.CurrentCount += Amount;

	if (State.CurrentCount >= Obj.TargetCount)
		State.bCompleted = true;

	CheckCompletion();
	//OnQuestUpdated.Broadcast(this);

	return true;
}

void UQuestInstance::ExportToSaveData(FQuestSaveData& OutSaveData) const
{
	OutSaveData.State = QuestState;
	OutSaveData.CurrentStep = CurrentStep;

	const int32 NumObjectives = ObjectiveStates.Num();
	OutSaveData.ObjectiveCounts.SetNum(NumObjectives);

	for (int32 i = 0; i < NumObjectives; ++i)
	{
		OutSaveData.ObjectiveCounts[i] = ObjectiveStates[i].CurrentCount;
	}
}

void UQuestInstance::CheckCompletion()
{
	for (const FObjectiveState& S : ObjectiveStates)
	{
		if (!S.bCompleted) return;
	}

	bCompleted = true;
}
