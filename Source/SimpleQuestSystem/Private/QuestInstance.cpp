// © Felix Courteau-Boutin All Right Reserved

#include "QuestInstance.h"

void UQuestInstance::Init(const UQuestDefinition* Def)
{
	Definition = Def;
	ObjectiveStates.SetNum(Definition->Objectives.Num());
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
