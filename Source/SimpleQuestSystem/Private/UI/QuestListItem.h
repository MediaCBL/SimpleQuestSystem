// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/QuestUserWidgetBase.h"
#include "QuestListItem.generated.h"

enum class EQObjectiveType : uint8;
class UTextBlock;

UCLASS()
class SIMPLEQUESTSYSTEM_API UQuestListItem : public UQuestUserWidgetBase, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void InitializeFromDefinition(const UQuestDefinition* InDefinition) override;
	static FString ObjectiveTypeToString(EQObjectiveType Type);
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ObjectiveText;
};
