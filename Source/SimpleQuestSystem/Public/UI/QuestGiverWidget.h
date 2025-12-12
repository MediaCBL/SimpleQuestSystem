// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "QuestUserWidgetBase.h"
#include "QuestGiverWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAcceptQuest);

UCLASS()
class SIMPLEQUESTSYSTEM_API UQuestGiverWidget : public UQuestUserWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Refresh_Implementation(UQuestInstance* Instance) override;

	UFUNCTION(BlueprintCallable, Category="Quest")
	void AcceptQuest();

	UPROPERTY(BlueprintAssignable, Category="Quest")
	FOnAcceptQuest OnAcceptQuest;
};

