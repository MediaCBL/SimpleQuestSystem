// © Felix Courteau-Boutin All Right Reserved

#include "Components/QuestGiverComponent.h"
#include "QuestManagerSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

UQuestGiverComponent::UQuestGiverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestGiverComponent::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	if (QuestGiverID.IsNone())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("QuestGiverComponent on %s has no QuestGiverID set"),
			*GetOwner()->GetName());
	}
#endif

	//TODO find a better way to Register quest with asset registery
	UQuestManagerSubsystem* Manager = GetQuestManager();
	if (Manager)
	{
		for (UQuestDefinition* Quest : QuestsOffered)
		{
			if (!Quest) continue;
			Manager->RegisterQuestDefinition(Quest);			
		}
	}
}

UQuestManagerSubsystem* UQuestGiverComponent::GetQuestManager() const
{
	if (UWorld* World = GetWorld())
	{
		return World->GetSubsystem<UQuestManagerSubsystem>();
	}
	return nullptr;
}

bool UQuestGiverComponent::CanOfferQuest(const UQuestDefinition* QuestDefinition) const
{
	if (!QuestDefinition || QuestGiverID.IsNone())
	{
		return false;
	}

	const UQuestManagerSubsystem* Manager = GetQuestManager();
	if (!Manager)
	{
		return false;
	}

	return Manager->CanGiveQuestFromGiver(
		QuestGiverID,
		QuestDefinition->QuestID
	) == EQuestAvailability::Available;
}

void UQuestGiverComponent::OfferQuestsToPlayer()
{
	if (QuestGiverID.IsNone())
	{
		return;
	}

	UQuestManagerSubsystem* Manager = GetQuestManager();
	if (!Manager)
	{
		return;
	}

	for (UQuestDefinition* QuestDefinition : QuestsOffered)
	{
		if (!QuestDefinition)
		{
			continue;
		}

		const EQuestAvailability Availability =
			Manager->CanGiveQuestFromGiver(
				QuestGiverID,
				QuestDefinition->QuestID
			);

		if (Availability == EQuestAvailability::Available)
		{
			Manager->GiveQuest(QuestGiverID, QuestDefinition->QuestID);
		}
	}
}