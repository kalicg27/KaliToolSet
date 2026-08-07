// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"



// 2. Struct to hold metadata for batch registration
struct FKaliTabDescriptor
{
	FName TabId;
	FText DisplayName;
	FText TooltipText;
	TFunction<TSharedRef<SWidget>()> WidgetBuilder;
};
class FKaliToolsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

#pragma region ContentBrowserMenuExtention

	void InitCBMenuExtention();
	TArray<FString> FolderPathsSelected;
	TSharedRef<FExtender> CustomCBMenuExtender(const TArray<FString>& SelectedPaths);

	void AddCBMenuEntry(class FMenuBuilder& MenuBuilder);
	void AddCBMenuEntryOwn(class FMenuBuilder& MenuBuilder);
	void OnMyOwnPluginButtonClicked();
	void OnImplementetionToolsButtonClicked();



#pragma endregion

#pragma region CustomEditorTab

	void RegistesterImplementetionToolsTab();
	
	TSharedRef<SDockTab> OnSpawnImplementetionToolsTab(const FSpawnTabArgs& SpawnTabArgs);
	//TSharedRef<SDockTab> ImplBlueprintSetUpTab(const FSpawnTabArgs& SpawnTabArgs);

	TArray < TSharedPtr <FAssetData>> GetAllSelectedAssets();

#pragma endregion
public:
#pragma region CustomToolsFunction

	static void SetForcedLOD(int32 LODLevel);
	static bool DeleteSingleAssetForList(const FAssetData& AssetDataToDelete);

#pragma endregion
};
