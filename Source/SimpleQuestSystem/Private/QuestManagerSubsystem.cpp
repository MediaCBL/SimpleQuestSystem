// © Felix Courteau-Boutin All Right Reserved

#include "QuestManagerSubsystem.h"

UQuestInstance* UQuestManagerSubsystem::StartQuest(const UQuestDefinition* QuestDef)
{
	UQuestInstance* Instance = NewObject<UQuestInstance>(this);
	Instance->Init(QuestDef);
	ActiveQuests.Add(Instance);
	return Instance;
}

void UQuestManagerSubsystem::ProgressObjective(FName QuestID, FName TargetID, int32 Amount)
{
	for (UQuestInstance* Quest : ActiveQuests)
	{
		if (Quest->Definition->QuestID != QuestID)
			continue;

		for (int32 i = 0; i < Quest->Definition->Objectives.Num(); i++)
		{
			const FQuestObjective& Obj = Quest->Definition->Objectives[i];

			if (Obj.TargetID == TargetID)
			{
				Quest->TryProgress(i, Amount);
			}
		}
	}
}

