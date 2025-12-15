// © Felix Courteau-Boutin All Right Reserved

#include "UI/QuestUserWidgetBase.h"
#include "QuestDefinition.h"
#include "QuestInstance.h"

void UQuestUserWidgetBase::InitializeFromDefinition(const UQuestDefinition* InDefinition)
{
	QuestDefinition = InDefinition;
}

void UQuestUserWidgetBase::InitializeFromInstance(UQuestInstance* InInstance)
{
	QuestInstance = InInstance;

	BindToInstance();
	Refresh(InInstance);
}

void UQuestUserWidgetBase::BindToInstance()
{
	if (QuestInstance)
	{
		//QuestInstance->OnQuestUpdated.AddDynamic(this, &UQuestUserWidgetBase::Refresh);
	}
}

void UQuestUserWidgetBase::Refresh_Implementation(UQuestInstance* UpdatedQuest)
{
	// Empty: child widgets override this
}

void UQuestUserWidgetBase::OnShow_Implementation()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UQuestUserWidgetBase::OnHide_Implementation()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

