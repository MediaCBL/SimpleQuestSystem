// © Felix Courteau-Boutin All Right Reserved

#include "Components/QuestGiverComponent.h"
#include "QuestManagerSubsystem.h"

void UQuestGiverComponent::OfferQuestsToPlayer(APlayerController* Player)
{
	if (!Player) return;

	UWorld* World = GetWorld();
	if (!World) return;

	UQuestManagerSubsystem* Manager = World->GetSubsystem<UQuestManagerSubsystem>();
	if (!Manager) return;

	for (UQuestDefinition* QuestDefinition : QuestsOffered)
	{
		if (QuestDefinition)
		{
			Manager->StartQuest(QuestDefinition);
		}
	}
}


