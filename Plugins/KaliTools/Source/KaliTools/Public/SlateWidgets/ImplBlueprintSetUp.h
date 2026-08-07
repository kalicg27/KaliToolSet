#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
struct FKaliBlueprintSetupData
{
	FString ResourceType;
	FName RowName;
};
class SImplBlueprintSetUp : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SImplBlueprintSetUp) {}
		SLATE_ARGUMENT(TArray<TSharedPtr<FAssetData>>, SelectedAssets)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	/** Stored assets list */
	TArray<TSharedPtr<FAssetData>> StoredAssets;

	/** Slate ListView widget handle */
	TSharedPtr<SListView<TSharedPtr<FAssetData>>> AssetListView;

	TSharedPtr<SVerticalBox> AssetParametersBox;

	//construct for main two parts
	TSharedRef<SWidget> ConstructAssetPanel();
	TSharedRef<SWidget> ConstructMainActionsPanel();

	//buttons to have main


	//might to delete if something goes wrong - no idea why this fuck is here
	TSharedRef<SWidget> MakeOptionComboWidget(TSharedPtr<FString> InItem);
	/** ListView Row Generator Callback */
	TSharedRef<ITableRow> OnGenerateAssetRow(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable);

	/** Refresh the UI list view */
	void RefreshListView();

	TArray<TSharedPtr<TArray<TSharedPtr<FString>>>> DynamicAllRowOptionsSources;

	// the buttons more here
	FReply OnLanguageButtonClicked();
	FReply OnReparentButtonClicked();
	FReply OnPresetButtonClicked(FName PresetName);
	FReply OnValidateAssetClicked();
	FReply OnCopySelectedClicked();
	FReply OnStartBPSetupClicked();

	// UI Field References (Used purely to read/set UI values)
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ResourceTypeComboBox;
	TArray<TSharedPtr<FString>> ResourceTypeOptions;


	// Data Helpers
	FKaliBlueprintSetupData GatherCurrentUIData() const;
	void SendDataToModulePlugin(const FKaliBlueprintSetupData& Data);
};