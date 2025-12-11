// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "QuestDefinition.h"
#include "QuestInstance.h"
#include "QuestManagerSubsystem.generated.h"

UCLASS()
class SIMPLEQUESTSYSTEM_API UQuestManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TArray<UQuestInstance*> ActiveQuests;

	UFUNCTION(BlueprintCallable)
	UQuestInstance* StartQuest(const UQuestDefinition* QuestDef);

	UFUNCTION(BlueprintCallable)
	void ProgressObjective(FName QuestID, FName TargetID, int32 Amount = 1);
};
