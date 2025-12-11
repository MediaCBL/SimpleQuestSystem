// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestGiverComponent.generated.h"


class UQuestDefinition;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMPLEQUESTSYSTEM_API UQuestGiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quests")
	TArray<UQuestDefinition*> QuestsOffered;

	UFUNCTION(BlueprintCallable, Category="Quests")
	void OfferQuestsToPlayer(APlayerController* Player);
};
