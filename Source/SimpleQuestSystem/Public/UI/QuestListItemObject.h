// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestListItemObject.generated.h"

class UQuestInstance;

UCLASS()
class SIMPLEQUESTSYSTEM_API UQuestListItemObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UQuestInstance> QuestInstance;
};
