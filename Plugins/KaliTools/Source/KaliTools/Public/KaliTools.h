#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "ImplPresetPluginData.h"

class UBlueprint;
class UCurveTable;
class UDataTable;
struct FAssetData;
class FMenuBuilder;
class FExtender;

// Struct for QuickTeleport World Partition locations
struct FLoadedPlaceData
{
	FString PlaceName;
	FVector Location;
	FRotator Rotation;
};

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
	void SetForcedLOD(int32 LODLevel);
	bool DeleteSingleAssetForList(const FAssetData& AssetDataToDelete);
	bool CopyParametersFromBlueprint(const FAssetData& AssetData, FKaliBlueprintSetupParams& OutParams);
	UCurveTable* FetchCurveTableFromBlueprint(const FAssetData& AssetData);
	TArray<FName> GetRowNamesFromCurveTable(UCurveTable* TargetCurveTable);
	UCurveTable* GetDefaultCurveTable();
	UDataTable* GetDefaultDataTable();

	/** Fetch CDO properties into string representation */
	bool FetchBlueprintProperties(UBlueprint* TargetBP, const TArray<FName>& PropertyNames, TMap<FName, FString>& OutValues);

	/** Apply string properties back to CDO, recompile, and save/checkout */
	bool ApplyBlueprintProperties(UBlueprint* TargetBP, const TMap<FName, FString>& PropertyValuesToApply);

	FKaliValidationResult ValidateBlueprintSetup(UBlueprint* TargetBP);
#pragma endregion

#pragma region ToolsForActionsTab
	void LoadWorldPartitionPlaces(const TArray<FString>& RawStrings, TArray<FLoadedPlaceData>& OutLoadedPlaces);
	void TeleportToLocation(const FVector& Location, const FRotator& Rotation);

private:
	bool ParseBugItString(const FString& InString, FVector& OutLocation, FRotator& OutRotation);
#pragma endregion
};
