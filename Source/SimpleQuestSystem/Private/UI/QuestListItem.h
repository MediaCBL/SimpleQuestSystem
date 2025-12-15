// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/QuestUserWidgetBase.h"
#include "QuestListItem.generated.h"

class UTextBlock;

UCLASS()
class SIMPLEQUESTSYSTEM_API UQuestListItem : public UQuestUserWidgetBase, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void InitializeFromDefinition(const UQuestDefinition* InDefinition) override;
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionText;
};
