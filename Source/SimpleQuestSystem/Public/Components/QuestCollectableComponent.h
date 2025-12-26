// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestCollectableComponent.generated.h"

UCLASS(ClassGroup=(Quest), meta=(BlueprintSpawnableComponent))
class SIMPLEQUESTSYSTEM_API UQuestCollectableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestCollectableComponent();

	/** Matches FQuestObjective::TargetID */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quest Collectable")
	FName TargetID;

	/** How many units this collectable gives */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quest Collectable")
	int32 Quantity = 1;

	/** Call when the object is collected */
	UFUNCTION(BlueprintCallable, Category="Quest Collectable")
	void Collect(AActor* Collector);

protected:
	virtual void BeginPlay() override;
};

