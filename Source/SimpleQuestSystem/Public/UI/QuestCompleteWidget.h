// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "QuestUserWidgetBase.h"
#include "QuestCompleteWidget.generated.h"

class UTextBlock;

UCLASS()
class SIMPLEQUESTSYSTEM_API UQuestCompleteWidget : public UQuestUserWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Refresh_Implementation(UQuestInstance* Instance) override;

	UPROPERTY(Transient, meta=(BindWidgetOptional))
	UTextBlock* QuestNameText;

	virtual void OnShow_Implementation() override;
};

