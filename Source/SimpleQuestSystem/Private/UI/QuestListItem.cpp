// © Felix Courteau-Boutin All Right Reserved

#include "QuestListItem.h"
#include "QuestDefinition.h"
#include "QuestInstance.h"
#include "Components/TextBlock.h"
#include "UI/QuestListItemObject.h"

void UQuestListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	const UQuestListItemObject* Item = Cast<UQuestListItemObject>(ListItemObject);
	if (!Item || !Item->QuestInstance)
	{
		return;
	}

	const UQuestDefinition* Definition = Item->QuestInstance->Definition;
	if (!Definition)
	{
		return;
	}

	if (!TitleText || !DescriptionText || !ObjectiveText) return;

	TitleText->SetText(Definition->QuestName);
	DescriptionText->SetText(Definition->Description);

	FString ObjectiveTextStr;
	for (int32 i = 0; i < Definition->Objectives.Num(); ++i)
	{
		const FQuestObjective& Objective = Definition->Objectives[i];

		const FString ObjectiveTypeStr =
			ObjectiveTypeToString(Objective.Type);

		// Example format:
		// "Collect Apple (5)"
		ObjectiveTextStr += FString::Printf(
			TEXT("%s %s (%d)"),
			*ObjectiveTypeStr,
			*Objective.TargetID.ToString(),
			Objective.TargetCount
		);

		if (i < Definition->Objectives.Num() - 1)
		{
			ObjectiveTextStr += TEXT("\n");
		}
	}

	ObjectiveText->SetText(FText::FromString(ObjectiveTextStr));
}

void UQuestListItem::InitializeFromDefinition(const UQuestDefinition* InDefinition)
{
	Super::InitializeFromDefinition(InDefinition);

	if (!TitleText || !DescriptionText || !InDefinition) return;
	
	TitleText->SetText(InDefinition->QuestName);
	DescriptionText->SetText(InDefinition->Description);

	FString ObjectiveTextStr;

	for (int32 i = 0; i < InDefinition->Objectives.Num(); ++i)
	{
		const FQuestObjective& Objective = InDefinition->Objectives[i];

		const FString ObjectiveTypeStr =
			ObjectiveTypeToString(Objective.Type);

		// Example format:
		// "Collect Apple (5)"
		ObjectiveTextStr += FString::Printf(
			TEXT("%s %s (%d)"),
			*ObjectiveTypeStr,
			*Objective.TargetID.ToString(),
			Objective.TargetCount
		);

		if (i < InDefinition->Objectives.Num() - 1)
		{
			ObjectiveTextStr += TEXT("\n");
		}
	}

	ObjectiveText->SetText(FText::FromString(ObjectiveTextStr));
}

FString UQuestListItem::ObjectiveTypeToString(EQObjectiveType Type)
{
	const UEnum* Enum = StaticEnum<EQObjectiveType>();
	if (!Enum)
	{
		return TEXT("Unknown");
	}

	return Enum->GetDisplayNameTextByValue(
		static_cast<int64>(Type)
	).ToString();
}
