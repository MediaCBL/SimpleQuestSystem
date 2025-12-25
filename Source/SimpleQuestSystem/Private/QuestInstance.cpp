// © Felix Courteau-Boutin All Right Reserved

#include "QuestInstance.h"

void UQuestInstance::Init(const UQuestDefinition* Def)
{
	Definition = Def;
	ObjectiveStates.SetNum(Definition->Objectives.Num());
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
	for (FObjectiveState& State : ObjectiveStates)
	{
		State.CurrentCount = 0;
		State.bCompleted = false;
	}

	// Restore based on quest state
	switch (SaveData.State)
	{
	case EQuestState::Inactive:
		bCompleted = false;
		break;

	case EQuestState::Active:
		{
			const int32 Step = SaveData.CurrentStep;

			// Mark previous steps as completed
			for (int32 i = 0; i < NumObjectives; ++i)
			{
				if (i < Step)
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

void UQuestInstance::CheckCompletion()
{
	for (const FObjectiveState& S : ObjectiveStates)
	{
		if (!S.bCompleted) return;
	}

	bCompleted = true;
}
