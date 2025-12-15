---

# SimpleQuestSystem Plugin

A lightweight, data-driven quest system designed for runtime use in Unreal Engine.
The plugin provides quest definitions, quest runtime instances, a world subsystem for global management, a flexible UI base class, and a component-based system for attaching quests to NPCs or interactive actors.

---

## Features

* Data-driven quests using DataAssets.
* Runtime quest instances with progress tracking.
* World subsystem for central quest management.
* Delegate-based update events.
* Modular UI base class for building quest-related widgets.
* Quest giver component for attaching missions to NPCs.
* Blueprint and C++ compatibility.

---

## Plugin Structure

```
SimpleQuestSystem/
 ├─ SimpleQuestSystem.uplugin
 ├─ Source/
 │   ├─ SimpleQuestSystem/
 │   │    ├─ SimpleQuestSystem.Build.cs
 │   │    ├─ Public/
 │   │    │     ├─ QuestTypes.h
 │   │    │     ├─ QuestDefinition.h
 │   │    │     ├─ QuestInstance.h
 │   │    │     ├─ QuestManagerSubsystem.h
 │   │    │     ├─ UI/QuestUserWidgetBase.h
 │   │    │     └─ Components/QuestGiverComponent.h
 │   │    └─ Private/
 │   │          ├─ QuestDefinition.cpp
 │   │          ├─ QuestInstance.cpp
 │   │          ├─ QuestManagerSubsystem.cpp
 │   │          ├─ UI/QuestUserWidgetBase.cpp
 │   │          └─ Components/QuestGiverComponent.cpp
```

---

## Core Concepts

### Quest Definitions

Quests are defined using `UQuestDefinition` DataAssets.
Each quest contains:

* A unique QuestID.
* Quest name and description.
* An array of objectives defined using `FQuestObjective`.

DataAssets allow designers to author quests without modifying code.

---

### Quest Runtime

`UQuestInstance` represents an active quest during gameplay. It contains:

* Reference to the `UQuestDefinition`.
* Array of objective runtime states.
* Completion state.
* A multicast delegate triggered when progress changes.

`TryProgress()` updates objectives and fires quest update events.

---

### Quest Manager Subsystem

`UQuestManagerSubsystem` is a `UWorldSubsystem` responsible for:

* Storing active quests.
* Starting new quests.
* Progressing objectives.
* Broadcasting quest events.

Accessed via:

```cpp
GetWorld()->GetSubsystem<UQuestManagerSubsystem>();
```

---

## User Interface Architecture

### Base UI Class

`UQuestUserWidgetBase` is a `UUserWidget` subclass used to build runtime quest UI.
It provides:

* Initialization from a quest definition or instance.
* A BlueprintNativeEvent `Refresh()` for updating display.
* Internal binding to quest update delegates.

Derived widgets may include:

* Quest giver UI
* Quest log UI
* Quest tracker UI
* Quest completion UI

---

## Attaching Quests to NPCs

Use `UQuestGiverComponent` to attach quests to any actor.

### Steps

1. Add `QuestGiverComponent` to an NPC Blueprint.
2. Assign one or more `UQuestDefinition` assets in the Details panel.
3. Call `OfferQuestsToPlayer(PlayerController)` when the player interacts.

The component retrieves the world subsystem and starts the quests defined on the NPC.

---

## Example: Offering a Quest

```cpp
UQuestGiverComponent* Giver = NPC->FindComponentByClass<UQuestGiverComponent>();
if (Giver)
{
    Giver->OfferQuestsToPlayer(PlayerController);
}
```

---

## Example: Progressing an Objective

```cpp
UQuestManagerSubsystem* Manager = GetWorld()->GetSubsystem<UQuestManagerSubsystem>();
Manager->ProgressObjective("MainQuest01", "Wolf", 1);
```

---

## Building Custom UI

Derived widgets override:

```cpp
virtual void Refresh_Implementation() override;
```

Blueprints can override the `Refresh` event for visual updates.

---

## Extending the Plugin

The system is designed for extension. Possible enhancements include:

* Quest requirements (level, items, previous quests).
* Quest rewards and completion effects.
* Dialogue integration.
* Quest boards.
* Save and load integrations.
* More complex objective types.

---

## Requirements

* Unreal Engine 5.5 or later.
* C++ project recommended for full integration.
* Blueprint projects supported for quest creation and UI logic.

---

## Installation

1. Place the `SimpleQuestSystem` folder inside your project's `Plugins/` directory.
2. Regenerate Visual Studio or Rider project files.
3. Build the project.
4. Enable the plugin in the Unreal Editor.

---

## Saving / Loading

This plugin does not implement a save system.

To persist quest state, call:

* `UQuestManagerSubsystem::ExportState`
* `UQuestManagerSubsystem::ImportState`

from your project’s save manager.

---

## License

MIT License

Copyright (c) 2025 MEDIA CBL

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

---
