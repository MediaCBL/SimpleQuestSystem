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

	// This replaces InitializeFromDefinition()
	TitleText->SetText(Definition->QuestName);
	DescriptionText->SetText(Definition->Description);
}

void UQuestListItem::InitializeFromDefinition(const UQuestDefinition* InDefinition)
{
	Super::InitializeFromDefinition(InDefinition);

	if (!TitleText || !DescriptionText || !InDefinition) return;
	
	TitleText->SetText(InDefinition->QuestName);
	DescriptionText->SetText(InDefinition->Description);
}
