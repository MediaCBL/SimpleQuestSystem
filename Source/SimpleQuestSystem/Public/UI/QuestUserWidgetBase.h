// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "QuestUserWidgetBase.generated.h"

class UQuestDefinition;
class UQuestInstance;

UCLASS(Abstract, Blueprintable)
class SIMPLEQUESTSYSTEM_API UQuestUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	/** Accepts quest definition (static info) */
	UFUNCTION(BlueprintCallable)
	virtual void InitializeFromDefinition(const UQuestDefinition* InDefinition);

	/** Accepts live quest instance (runtime info) */
	UFUNCTION(BlueprintCallable)
	virtual void InitializeFromInstance(UQuestInstance* InInstance);

	/** Called anytime quest instance updates */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Refresh(UQuestInstance* UpdatedQuest);

	/** Called when UI is shown */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnShow();

	/** Called when UI is hidden */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHide();

protected:

	UPROPERTY(BlueprintReadOnly)
	const UQuestDefinition* QuestDefinition;

	UPROPERTY(BlueprintReadOnly)
	UQuestInstance* QuestInstance;

private:
	/** Bind delegate once */
	void BindToInstance();
};
