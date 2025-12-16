// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestGiverComponent.generated.h"

class UQuestDefinition;
class UQuestManagerSubsystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMPLEQUESTSYSTEM_API UQuestGiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestGiverComponent();

	/** Stable ID used for save + quest giver tracking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quest Giver")
	FName QuestGiverID;

	/** Quests this NPC can offer */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quest Giver")
	TArray<UQuestDefinition*> QuestsOffered;

	/** Offer all available quests (usually triggered by interaction or dialog) */
	UFUNCTION(BlueprintCallable, Category="Quest Giver")
	void OfferQuestsToPlayer();

	/** Check availability for UI / dialog branching */
	UFUNCTION(BlueprintPure, Category="Quest Giver")
	bool CanOfferQuest(const UQuestDefinition* QuestDefinition) const;

protected:
	virtual void BeginPlay() override;

private:
	UQuestManagerSubsystem* GetQuestManager() const;
};
