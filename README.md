# KaliToolSet — Unreal Engine 5 Editor Utility Plugin

**KaliToolSet** is a C++ and Slate-based Unreal Engine 5 Editor plugin designed to accelerate level design, technical art, and gameplay prototyping workflows. It centralizes batch Blueprint property editing, template-based Blueprint generation, and one-click viewport visualization toggles into a unified, dockable Editor window.

## Demo Video

[![KaliToolSet Demo Video](https://vumbnail.com/1248631041.jpg)](https://vimeo.com/1248631041 "Watch the KaliToolSet Demo on Vimeo")

*Click the preview above or [watch the walkthrough directly on Vimeo](https://vimeo.com/1248631041).*

---
---

## Key Features

### 1. Blueprint Batch Property Modifier & Template Generator
* **Content Browser Integration**: Extends the Content Browser asset context menu (`FContentBrowserMenuExtender`) to launch the **Implementation Tools** window directly on selected assets.
* **Live Blueprint CDO Editing**: Fetches selected Blueprint assets, inspects their Class Default Objects (CDO) and component hierarchies (`USimpleConstructionScript`), and exposes editable properties dynamically via `IDetailsView`.
* **Batch Applying**: Applies modified actor defaults (e.g., `AutoReceiveInput`, `SpawnCollisionHandlingMethod`, `InitialLifeSpan`) and sub-component settings (e.g., `UStaticMeshComponent` materials, `UBoxComponent` collision presets and extents) across multiple Blueprints simultaneously, marking packages dirty and handling safe compilation/saving.
* **Template-Based Blueprint Creation**: One-click generation of categorized gameplay Blueprints (`Weapons`, `Ship`, `Resources`, `Pickups`, `Buildings`, `AI`) alongside direct access to project DataTables.

### 2. Viewport Visual & Debugging Tools
* **Instant Viewport Overrides**: Toggle `Wireframe`, `Collision`, and `Bounds` rendering flags directly on the active `FEditorViewportClient` without navigating nested viewport menus.

### 3. Modular Multi-Tab Slate Architecture
* Built using pure C++ **Slate** (`SCompoundWidget`, `SDockTab`, `SWidgetSwitcher`, `SScrollBox`) for native editor performance and responsive docking.
* Organized into dedicated workflow tabs: **Blueprint Setup**, **Static Mesh Setup**, **Textures Setup**, **Actions**, and **Visuals**.

---

## Technical Architecture & C++ Highlights

This project demonstrates practical experience with Unreal Engine's Editor internals and C++ architecture:

* **Slate UI Framework**: Custom compound widgets (`SCompoundWidget`), dynamic tab switching, custom styling (`FAppStyle`), and clean delegate binding (`FOnClicked`, `FOnTextChanged`).
* **Unreal Property System & Reflection**: Programmatic inspection and modification of `FProperty` / `UObject` defaults on Blueprint CDOs (`UBlueprint::GeneratedClass->GetDefaultObject()`), including traversing `USCS_Node` component trees.
* **Editor Modules & Extensibility**: Proper separation of `Editor` module dependencies (`UnrealEd`, `Slate`, `SlateCore`, `PropertyEditor`, `AssetTools`, `ContentBrowser`, `EditorStyle`).
* **Asset Management**: Safe interaction with `FAssetRegistryModule`, `FAssetToolsModule`, `FKismetEditorUtilities`, and `UPackage` dirty-state management.

---

## Installation & Usage

1. Clone or download this repository into your Unreal Engine project's `Plugins/` directory:
   ```bash
   git clone [https://github.com/kalicg27/KaliToolSet.git](https://github.com/kalicg27/KaliToolSet.git) YourProject/Plugins/KaliToolSet
