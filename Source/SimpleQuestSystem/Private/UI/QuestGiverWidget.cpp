// © Felix Courteau-Boutin All Right Reserved

#include "UI/QuestGiverWidget.h"

void UQuestGiverWidget::Refresh_Implementation(UQuestInstance* Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Refreshing QuestGiverWidget"));
}

void UQuestGiverWidget::AcceptQuest()
{
	OnAcceptQuest.Broadcast();
	// Update subsystem quest manager 
}
