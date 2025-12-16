// © Felix Courteau-Boutin All Right Reserved

#include "QuestManagerSubsystem.h"
#include "Engine/AssetManager.h"

void UQuestManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Auto-register definitions (Asset Manager scan).
	/*UAssetManager& AssetManager = UAssetManager::Get();
	UAssetManager::Get().CallOrRegister_OnCompletedInitialScan(
		FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &UQuestManagerSubsystem::RegisterAllQuestDefinitions)
	);*/
}

void UQuestManagerSubsystem::RegisterAllQuestDefinitions()
{
	UAssetManager& AssetManager = UAssetManager::Get();

	TArray<FPrimaryAssetId> QuestAssetIds;
	AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("Quest"), QuestAssetIds);

	for (const FPrimaryAssetId& AssetId : QuestAssetIds)
	{
		if (UQuestDefinition* Def =
			Cast<UQuestDefinition>(AssetManager.GetPrimaryAssetObject(AssetId)))
		{
			RegisterQuestDefinition(Def);
		}
	}
}

void UQuestManagerSubsystem::Deinitialize()
{
	QuestDefinitions.Empty();
	QuestStates.Empty();
	QuestGiverStates.Empty();

	Super::Deinitialize();
}

// -------------------- Definition Registry --------------------

void UQuestManagerSubsystem::RegisterQuestDefinition(UQuestDefinition* Definition)
{
	if (!Definition)
	{
		return;
	}
	if (Definition->QuestID.IsNone())
	{
		// You can UE_LOG here if desired.
		return;
	}

	QuestDefinitions.FindOrAdd(Definition->QuestID) = Definition;
}

void UQuestManagerSubsystem::RegisterQuestDefinitions(const TArray<UQuestDefinition*>& Definitions)
{
	for (UQuestDefinition* Def : Definitions)
	{
		RegisterQuestDefinition(Def);
	}
}

UQuestDefinition* UQuestManagerSubsystem::GetQuestDefinition(FName QuestID) const
{
	if (const TObjectPtr<UQuestDefinition>* Found = QuestDefinitions.Find(QuestID))
	{
		return Found->Get();
	}
	return nullptr;
}

// -------------------- Availability API --------------------

EQuestAvailability UQuestManagerSubsystem::CanGiveQuest(FName QuestID) const
{
	const UQuestDefinition* Def = GetQuestDefinition(QuestID);
	if (!Def)
	{
		return EQuestAvailability::InvalidQuest;
	}

	const FQuestSaveData* State = FindQuestStateConst(QuestID);
	const EQuestState CurrentState = State ? State->State : EQuestState::Inactive;

	if (CurrentState == EQuestState::Active)
	{
		return EQuestAvailability::AlreadyActive;
	}

	// One-time quest: block if ever completed
	if (!Def->bIsRepeatable)
	{
		if (State && State->TimesCompleted > 0)
		{
			return EQuestAvailability::AlreadyCompleted;
		}
	}

	if (!ArePrerequisitesMet(Def))
	{
		return EQuestAvailability::LockedByPrerequisite;
	}

	// Repeatable cooldown check (only meaningful if completed before)
	if (Def->bIsRepeatable && State && State->TimesCompleted > 0)
	{
		if (!IsRepeatCooldownSatisfied(Def, *State))
		{
			return EQuestAvailability::OnCooldown;
		}
	}

	return EQuestAvailability::Available;
}

EQuestAvailability UQuestManagerSubsystem::CanGiveQuestFromGiver(FName QuestGiverID, FName QuestID) const
{
	// Base availability first
	EQuestAvailability Base = CanGiveQuest(QuestID);
	if (Base != EQuestAvailability::Available)
	{
		return Base;
	}

	// If giver has already given this quest, block (common for story/one-time).
	// For repeatables, you may choose to allow the same giver to re-offer after cooldown.
	// This skeleton blocks by default only if giver marked "Given" AND quest is one-time.
	const UQuestDefinition* Def = GetQuestDefinition(QuestID);
	if (!Def)
	{
		return EQuestAvailability::InvalidQuest;
	}

	if (HasQuestGiverGivenQuest(QuestGiverID, QuestID))
	{
		// For repeatables you might return Available if your design wants re-offer.
		// Here we treat "Given" as a per-giver lock for one-time quests.
		if (!Def->bIsRepeatable)
		{
			return EQuestAvailability::AlreadyGivenByThisGiver;
		}
	}

	return EQuestAvailability::Available;
}

// -------------------- State Queries --------------------

bool UQuestManagerSubsystem::IsQuestActive(FName QuestID) const
{
	return GetQuestState(QuestID) == EQuestState::Active;
}

bool UQuestManagerSubsystem::IsQuestCompleted(FName QuestID) const
{
	return GetQuestState(QuestID) == EQuestState::Completed;
}

bool UQuestManagerSubsystem::HasQuestEverCompleted(FName QuestID) const
{
	if (const FQuestSaveData* State = FindQuestStateConst(QuestID))
	{
		return State->TimesCompleted > 0;
	}
	return false;
}

EQuestState UQuestManagerSubsystem::GetQuestState(FName QuestID) const
{
	if (const FQuestSaveData* State = FindQuestStateConst(QuestID))
	{
		return State->State;
	}
	return EQuestState::Inactive;
}

int32 UQuestManagerSubsystem::GetQuestCurrentStep(FName QuestID) const
{
	if (const FQuestSaveData* State = FindQuestStateConst(QuestID))
	{
		return State->CurrentStep;
	}
	return INDEX_NONE;
}

int32 UQuestManagerSubsystem::GetQuestTimesCompleted(FName QuestID) const
{
	if (const FQuestSaveData* State = FindQuestStateConst(QuestID))
	{
		return State->TimesCompleted;
	}
	return 0;
}

TArray<FName> UQuestManagerSubsystem::GetAllActiveQuests() const
{
	TArray<FName> Out;
	Out.Reserve(QuestStates.Num());
	for (const auto& KVP : QuestStates)
	{
		if (KVP.Value.State == EQuestState::Active)
		{
			Out.Add(KVP.Key);
		}
	}
	return Out;
}

TArray<FName> UQuestManagerSubsystem::GetAllCompletedQuests() const
{
	TArray<FName> Out;
	Out.Reserve(QuestStates.Num());
	for (const auto& KVP : QuestStates)
	{
		if (KVP.Value.TimesCompleted > 0 || KVP.Value.State == EQuestState::Completed)
		{
			Out.Add(KVP.Key);
		}
	}
	return Out;
}

// -------------------- Quest Giver Queries --------------------

bool UQuestManagerSubsystem::HasQuestGiverGivenQuest(FName QuestGiverID, FName QuestID) const
{
	if (const FQuestGiverSaveData* Giver = FindGiverStateConst(QuestGiverID))
	{
		if (const EQuestGiverQuestState* State = Giver->QuestStates.Find(QuestID))
		{
			return (*State == EQuestGiverQuestState::Given);
		}
	}
	return false;
}

TArray<FName> UQuestManagerSubsystem::GetQuestsGivenByGiver(FName QuestGiverID) const
{
	TArray<FName> Out;
	if (const FQuestGiverSaveData* Giver = FindGiverStateConst(QuestGiverID))
	{
		for (const auto& KVP : Giver->QuestStates)
		{
			if (KVP.Value == EQuestGiverQuestState::Given)
			{
				Out.Add(KVP.Key);
			}
		}
	}
	return Out;
}

// -------------------- Actions --------------------

bool UQuestManagerSubsystem::GiveQuest(FName QuestGiverID, FName QuestID)
{
	const EQuestAvailability Avail = CanGiveQuestFromGiver(QuestGiverID, QuestID);
	if (Avail != EQuestAvailability::Available)
	{
		return false;
	}

	// Mark giver relationship
	{
		FQuestGiverSaveData& Giver = FindOrAddGiverState(QuestGiverID);
		Giver.QuestStates.FindOrAdd(QuestID) = EQuestGiverQuestState::Given;
	}

	OnQuestGiven.Broadcast(QuestGiverID, QuestID);

	// Activate quest
	if (!StartQuest(QuestID))
	{
		// If start fails, you may want to roll back giver state. Up to your design.
		return false;
	}

	return true;
}

bool UQuestManagerSubsystem::StartQuest(FName QuestID)
{
	const UQuestDefinition* Def = GetQuestDefinition(QuestID);
	if (!Def)
	{
		return false;
	}

	// If repeatable and currently completed/inactive, ensure attempt reset.
	FQuestSaveData& State = FindOrAddQuestState(QuestID);

	if (State.State == EQuestState::Active)
	{
		return false;
	}

	// One-time: do not allow re-start after completion
	if (!Def->bIsRepeatable && State.TimesCompleted > 0)
	{
		return false;
	}

	// Repeatable cooldown gate
	if (Def->bIsRepeatable && State.TimesCompleted > 0)
	{
		if (!IsRepeatCooldownSatisfied(Def, State))
		{
			return false;
		}
	}

	// Reset attempt fields
	State.State = EQuestState::Active;
	State.CurrentStep = 0;

	OnQuestStarted.Broadcast(QuestID);
	OnQuestUpdated.Broadcast(QuestID);
	return true;
}

bool UQuestManagerSubsystem::CompleteQuest(FName QuestID)
{
	FQuestSaveData* StatePtr = QuestStates.Find(QuestID);
	if (!StatePtr)
	{
		// Completing a quest that never existed is suspicious; you can decide to create it.
		return false;
	}

	FQuestSaveData& State = *StatePtr;
	if (State.State != EQuestState::Active)
	{
		return false;
	}

	State.State = EQuestState::Completed;
	State.TimesCompleted += 1;
	State.LastCompletedTime = FDateTime::UtcNow();
	State.CurrentStep = INDEX_NONE;

	OnQuestCompleted.Broadcast(QuestID);
	OnQuestUpdated.Broadcast(QuestID);
	return true;
}

bool UQuestManagerSubsystem::FailQuest(FName QuestID)
{
	FQuestSaveData* StatePtr = QuestStates.Find(QuestID);
	if (!StatePtr || StatePtr->State != EQuestState::Active)
	{
		return false;
	}

	StatePtr->State = EQuestState::Failed;
	StatePtr->CurrentStep = INDEX_NONE;

	OnQuestFailed.Broadcast(QuestID);
	OnQuestUpdated.Broadcast(QuestID);
	return true;
}

bool UQuestManagerSubsystem::AbandonQuest(FName QuestID)
{
	FQuestSaveData* StatePtr = QuestStates.Find(QuestID);
	if (!StatePtr || StatePtr->State != EQuestState::Active)
	{
		return false;
	}

	// Abandon returns to Inactive (history intact).
	StatePtr->State = EQuestState::Inactive;
	StatePtr->CurrentStep = INDEX_NONE;

	OnQuestUpdated.Broadcast(QuestID);
	return true;
}

bool UQuestManagerSubsystem::ResetQuestAttempt(FName QuestID)
{
	FQuestSaveData* StatePtr = QuestStates.Find(QuestID);
	if (!StatePtr)
	{
		return false;
	}

	// Reset attempt, keep history.
	StatePtr->State = EQuestState::Inactive;
	StatePtr->CurrentStep = INDEX_NONE;

	OnQuestUpdated.Broadcast(QuestID);
	return true;
}

// -------------------- Save Hooks --------------------

void UQuestManagerSubsystem::ExportState(TMap<FName, FQuestSaveData>& OutQuestStates, TMap<FName, FQuestGiverSaveData>& OutGiverStates) const
{
	OutQuestStates = QuestStates;
	OutGiverStates = QuestGiverStates;
}

void UQuestManagerSubsystem::ImportState(const TMap<FName, FQuestSaveData>& InQuestStates, const TMap<FName, FQuestGiverSaveData>& InGiverStates)
{
	QuestStates = InQuestStates;
	QuestGiverStates = InGiverStates;

	// Notify UI that state changed (optional: batch event)
	for (const auto& KVP : QuestStates)
	{
		OnQuestUpdated.Broadcast(KVP.Key);
	}
}

// -------------------- Internals --------------------

const FQuestSaveData* UQuestManagerSubsystem::FindQuestStateConst(FName QuestID) const
{
	return QuestStates.Find(QuestID);
}

FQuestSaveData& UQuestManagerSubsystem::FindOrAddQuestState(FName QuestID)
{
	return QuestStates.FindOrAdd(QuestID);
}

const FQuestGiverSaveData* UQuestManagerSubsystem::FindGiverStateConst(FName QuestGiverID) const
{
	return QuestGiverStates.Find(QuestGiverID);
}

FQuestGiverSaveData& UQuestManagerSubsystem::FindOrAddGiverState(FName QuestGiverID)
{
	return QuestGiverStates.FindOrAdd(QuestGiverID);
}

bool UQuestManagerSubsystem::ArePrerequisitesMet(const UQuestDefinition* Def) const
{
	if (!Def)
	{
		return false;
	}

	for (const FName& RequiredQuestID : Def->RequiredCompletedQuests)
	{
		if (!HasQuestEverCompleted(RequiredQuestID))
		{
			return false;
		}
	}
	return true;
}

bool UQuestManagerSubsystem::IsRepeatCooldownSatisfied(const UQuestDefinition* Def, const FQuestSaveData& State) const
{
	if (!Def || !Def->bIsRepeatable)
	{
		return true;
	}

	const float Cooldown = Def->RepeatCooldownSeconds;
	if (Cooldown <= 0.0f)
	{
		return true;
	}

	if (State.TimesCompleted <= 0)
	{
		return true;
	}

	// If LastCompletedTime is unset, allow.
	if (State.LastCompletedTime.GetTicks() <= 0)
	{
		return true;
	}

	const FTimespan Since = FDateTime::UtcNow() - State.LastCompletedTime;
	return Since.GetTotalSeconds() >= Cooldown;
}
/*UQuestInstance* UQuestManagerSubsystem::StartQuest(const UQuestDefinition* QuestDef)
{
	UQuestInstance* Instance = NewObject<UQuestInstance>(this);
	Instance->Init(QuestDef);
	ActiveQuests.Add(Instance);
	return Instance;
}*/

void UQuestManagerSubsystem::ProgressObjective(FName QuestID, FName TargetID, int32 Amount)
{
	for (UQuestInstance* Quest : ActiveQuests)
	{
		if (Quest->Definition->QuestID != QuestID)
			continue;

		for (int32 i = 0; i < Quest->Definition->Objectives.Num(); i++)
		{
			const FQuestObjective& Obj = Quest->Definition->Objectives[i];

			if (Obj.TargetID == TargetID)
			{
				Quest->TryProgress(i, Amount);
			}
		}
	}
}

