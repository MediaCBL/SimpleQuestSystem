// © Felix Courteau-Boutin All Right Reserved

#include "UI/QuestLogWidget.h"
#include "QuestManagerSubsystem.h"
#include "Components/ListView.h"
#include "UI/QuestListItemObject.h"

void UQuestLogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!QuestList)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UQuestManagerSubsystem* QuestSubsystem = World->GetSubsystem<UQuestManagerSubsystem>();

	if (!QuestSubsystem)
	{
		return;
	}

	QuestList->ClearListItems();

	for (UQuestInstance* Quest : QuestSubsystem->ActiveQuests)
	{
		if (!Quest) continue;

		UQuestListItemObject* ItemObject = NewObject<UQuestListItemObject>(this);

		ItemObject->QuestInstance = Quest;

		QuestList->AddItem(ItemObject);
	}
}
