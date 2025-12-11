// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "QuestUserWidgetBase.h"
#include "QuestGiverWidget.generated.h"

UCLASS()
class SIMPLEQUESTSYSTEM_API UQuestGiverWidget : public UQuestUserWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Refresh_Implementation(UQuestInstance* Instance) override;
};

