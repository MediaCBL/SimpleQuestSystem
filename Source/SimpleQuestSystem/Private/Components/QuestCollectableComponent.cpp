// © Felix Courteau-Boutin All Right Reserved

#include "Components/QuestCollectableComponent.h"
#include "QuestManagerSubsystem.h"
#include "GameFramework/Actor.h"

UQuestCollectableComponent::UQuestCollectableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestCollectableComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQuestCollectableComponent::Collect(AActor* Collector)
{
	if (!GetWorld())
		return;

	UQuestManagerSubsystem* QuestManager =
		GetWorld()->GetSubsystem<UQuestManagerSubsystem>();

	if (!QuestManager)
		return;

	QuestManager->ProgressObjectiveByTarget(TargetID, Quantity);
}


