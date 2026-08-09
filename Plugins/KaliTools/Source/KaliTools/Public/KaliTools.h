// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "ImplPresetPluginData.h"

// Forward Declarations
class UBlueprint;
class UCurveTable;
class UDataTable;
struct FAssetData;
class FMenuBuilder;
class FExtender;
struct FKaliBlueprintSetupParams;

// Data Structure for Loaded Places
struct FLoadedPlaceData
{
	FString PlaceName;
	FVector Location;
	FRotator Rotation;
};

// Struct to hold metadata for batch registration
struct FKaliTabDescriptor
{
	FName TabId;
	FText DisplayName;
	FText TooltipText;
	TFunction<TSharedRef<SWidget>()> WidgetBuilder;
};

struct FKaliValidationResult
{
	bool bIsValid = true;
	TArray<FString> ValidationErrors;
	TArray<FString> ValidationWarnings;
};

class FKaliToolsModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static FKaliToolsModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FKaliToolsModule>("KaliTools");
	}

private:
#pragma region ContentBrowserMenuExtention
	void InitCBMenuExtention();
	TArray<FString> FolderPathsSelected;
	TSharedRef<FExtender> CustomCBMenuExtender(const TArray<FString>& SelectedPaths);

	void AddCBMenuEntry(FMenuBuilder& MenuBuilder);
	void AddCBMenuEntryOwn(FMenuBuilder& MenuBuilder);
	void OnMyOwnPluginButtonClicked();
	void OnImplementetionToolsButtonClicked();
#pragma endregion

#pragma region CustomEditorTab
	void RegistesterImplementetionToolsTab();
	TSharedRef<SDockTab> OnSpawnImplementetionToolsTab(const FSpawnTabArgs& SpawnTabArgs);
	TArray<TSharedPtr<FAssetData>> GetAllSelectedAssets();
#pragma endregion

public:
#pragma region CustomToolsFunction
	static void SetForcedLOD(int32 LODLevel);
	static bool DeleteSingleAssetForList(const FAssetData& AssetDataToDelete);
	bool CopyParametersFromBlueprint(const FAssetData& AssetData, FKaliBlueprintSetupParams& OutParams);
	UCurveTable* FetchCurveTableFromBlueprint(const FAssetData& AssetData);
	TArray<FName> GetRowNamesFromCurveTable(UCurveTable* TargetCurveTable);
	UCurveTable* GetDefaultCurveTable();
	UDataTable* GetDefaultDataTable();

	/** Fetch CDO properties into string representation */
	static bool FetchBlueprintProperties(UBlueprint* TargetBP, const TArray<FName>& PropertyNames, TMap<FName, FString>& OutValues);

	/** Apply string properties back to CDO, recompile, and save/checkout */
	static bool ApplyBlueprintProperties(UBlueprint* TargetBP, const TMap<FName, FString>& PropertyValuesToApply);

	static FKaliValidationResult ValidateBlueprintSetup(UBlueprint* TargetBP);

	void LoadWorldPartitionPlaces(const TArray<FString>& RawStrings, TArray<FLoadedPlaceData>& OutLoadedPlaces);
	static void TeleportToLocation(const FVector& Location, const FRotator& Rotation);
	TArray<FBox> PreviouslyLoadedRegions;
#pragma endregion

private:
	bool ParseBugItString(const FString& InString, FVector& OutLocation, FRotator& OutRotation);
};