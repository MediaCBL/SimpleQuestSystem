// © Felix Courteau-Boutin All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "QuestDefinition.h"
#include "QuestInstance.h"
#include "QuestManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestGiven, FName, QuestGiverID, FName, QuestID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestStarted, FName, QuestID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, FName, QuestID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestFailed, FName, QuestID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdated, FName, QuestID);

/**
 * Quest Manager Subsystem (authoritative runtime state + query/command API).
 * - NPCs / Dialog / UI query this subsystem only (never touch SaveGame directly).
 * - Persistent save/load should be implemented by a separate SaveGame manager that
 *   calls Export/Import methods (added below).
 */
UCLASS()
class SIMPLEQUESTSYSTEM_API UQuestManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
	UFUNCTION(BlueprintCallable, Category="Quest|Definitions")
	void RegisterQuestDefinition(UQuestDefinition* Definition);

	UFUNCTION(BlueprintCallable, Category="Quest|Definitions")
	void RegisterQuestDefinitions(const TArray<UQuestDefinition*>& Definitions);

	UFUNCTION(BlueprintPure, Category="Quest|Definitions")
	UQuestDefinition* GetQuestDefinition(FName QuestID) const;

	UFUNCTION(BlueprintPure, Category="Quest|Availability")
	EQuestAvailability CanGiveQuest(FName QuestID) const;

	UFUNCTION(BlueprintPure, Category="Quest|Availability")
	EQuestAvailability CanGiveQuestFromGiver(FName QuestGiverID, FName QuestID) const;

	UFUNCTION(BlueprintPure, Category="Quest|State")
	bool IsQuestActive(FName QuestID) const;

	UFUNCTION(BlueprintPure, Category="Quest|State")
	bool IsQuestCompleted(FName QuestID) const;

	/** For repeatables: true if TimesCompleted > 0 (even if currently inactive). */
	UFUNCTION(BlueprintPure, Category="Quest|State")
	bool HasQuestEverCompleted(FName QuestID) const;

	UFUNCTION(BlueprintPure, Category="Quest|State")
	EQuestState GetQuestState(FName QuestID) const;

	UFUNCTION(BlueprintPure, Category="Quest|State")
	int32 GetQuestCurrentStep(FName QuestID) const;

	UFUNCTION(BlueprintPure, Category="Quest|State")
	int32 GetQuestTimesCompleted(FName QuestID) const;

	UFUNCTION(BlueprintPure, Category="Quest|State")
	TArray<FName> GetAllActiveQuests() const;

	UFUNCTION(BlueprintPure, Category="Quest|State")
	TArray<FName> GetAllCompletedQuests() const;

	UFUNCTION(BlueprintPure, Category="Quest|Giver")
	bool HasQuestGiverGivenQuest(FName QuestGiverID, FName QuestID) const;

	UFUNCTION(BlueprintPure, Category="Quest|Giver")
	TArray<FName> GetQuestsGivenByGiver(FName QuestGiverID) const;

	/** Main entry point for NPCs: validates, marks giver state, activates quest, broadcasts events. */
	UFUNCTION(BlueprintCallable, Category="Quest|Actions")
	bool GiveQuest(FName QuestGiverID, FName QuestID);

	UFUNCTION(BlueprintCallable, Category="Quest|Actions")
	bool StartQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, Category="Quest|Actions")
	bool CompleteQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, Category="Quest|Actions")
	bool FailQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, Category="Quest|Actions")
	bool AbandonQuest(FName QuestID);

	/** For repeatables: resets current attempt (keeps history like TimesCompleted). */
	UFUNCTION(BlueprintCallable, Category="Quest|Actions")
	bool ResetQuestAttempt(FName QuestID);

	UFUNCTION(BlueprintCallable)
	void ProgressObjective(FName QuestID, FName TargetID, int32 Amount = 1);

	UFUNCTION(BlueprintCallable)
	void ProgressObjectiveByTarget(FName TargetID, int32 Amount);

	UFUNCTION(BlueprintCallable)
	void SyncQuestSaveData(const UQuestInstance* QuestInstance);

	/** Export current quest + giver states to be written into a SaveGame object. */
	void ExportState(TMap<FName, FQuestSaveData>& OutQuestStates,
	                 TMap<FName, FQuestGiverSaveData>& OutGiverStates) const;

	/** Import quest + giver states after loading SaveGame. */
	void ImportState(const TMap<FName, FQuestSaveData>& InQuestStates,
	                 const TMap<FName, FQuestGiverSaveData>& InGiverStates);

	UPROPERTY(BlueprintAssignable, Category="Quest|Events")
	FOnQuestGiven OnQuestGiven;

	UPROPERTY(BlueprintAssignable, Category="Quest|Events")
	FOnQuestStarted OnQuestStarted;

	UPROPERTY(BlueprintAssignable, Category="Quest|Events")
	FOnQuestCompleted OnQuestCompleted;

	UPROPERTY(BlueprintAssignable, Category="Quest|Events")
	FOnQuestFailed OnQuestFailed;

	UPROPERTY(BlueprintAssignable, Category="Quest|Events")
	FOnQuestUpdated OnQuestUpdated;

	UPROPERTY(BlueprintReadOnly)
	TArray<UQuestInstance*> ActiveQuests;
	UPROPERTY(BlueprintReadOnly)
	TArray<UQuestInstance*> CompletedQuests;

private:
	const FQuestSaveData* FindQuestStateConst(FName QuestID) const;
	FQuestSaveData& FindOrAddQuestState(FName QuestID);
	const FQuestGiverSaveData* FindGiverStateConst(FName QuestGiverID) const;
	FQuestGiverSaveData& FindOrAddGiverState(FName QuestGiverID);
	bool ArePrerequisitesMet(const UQuestDefinition* Def) const;
	bool IsRepeatCooldownSatisfied(const UQuestDefinition* Def, const FQuestSaveData& State) const;
	void RegisterAllQuestDefinitions();

	// Definition registry: QuestID -> Definition
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<UQuestDefinition>> QuestDefinitions;

	// Runtime state (also used as save source)
	UPROPERTY(Transient)
	TMap<FName, FQuestSaveData> QuestStates;

	UPROPERTY(Transient)
	TMap<FName, FQuestGiverSaveData> QuestGiverStates;
};
