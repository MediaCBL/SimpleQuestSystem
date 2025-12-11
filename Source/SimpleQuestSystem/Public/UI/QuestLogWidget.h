// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "QuestUserWidgetBase.h"
#include "QuestLogWidget.generated.h"

UCLASS()
class SIMPLEQUESTSYSTEM_API UQuestLogWidget : public UQuestUserWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Refresh_Implementation(UQuestInstance* Instance) override;
};
