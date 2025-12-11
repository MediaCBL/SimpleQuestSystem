// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestTypes.h"
#include "QuestDefinition.generated.h"

UCLASS(BlueprintType)
class SIMPLEQUESTSYSTEM_API UQuestDefinition : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FQuestObjective> Objectives;
};
