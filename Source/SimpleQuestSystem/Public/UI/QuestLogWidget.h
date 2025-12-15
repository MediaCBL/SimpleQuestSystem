// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestLogWidget.generated.h"

class UListView;
UCLASS()
class SIMPLEQUESTSYSTEM_API UQuestLogWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UListView* QuestList;

};
