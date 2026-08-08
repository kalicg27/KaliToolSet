#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "AssetRegistry/AssetData.h"

#include "ImplPresetPluginData.h"

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

	// Panel Builders
	TSharedRef<SWidget> ConstructAssetPanel();
	TSharedRef<SWidget> ConstructMainActionsPanel();

	// ListView Helpers
	TSharedRef<SWidget> MakeOptionComboWidget(TSharedPtr<FString> InItem);
	TSharedRef<ITableRow> OnGenerateAssetRow(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable);
	void RefreshListView();

	// Dynamic Attributes & Display Data
	TArray<TSharedPtr<TArray<TSharedPtr<FString>>>> DynamicAllRowOptionsSources;
	FAssetData CurrentSelectedAssetData;
	FString CurrentSelectedParentClassName = TEXT("None");
	FKaliBlueprintSetupParams CurrentSetupParams;

	FText GetSelectedAssetNameText() const;
	FText GetSelectedParentClassNameText() const;
	const FSlateBrush* GetSelectedAssetIcon() const;

	// Button Event Callbacks
	FReply OnLanguageButtonClicked();
	FReply OnReparentButtonClicked();
	FReply OnPresetButtonClicked(FName PresetName);
	FReply OnValidateAssetClicked();
	FReply OnCopySelectedClicked();
	FReply OnStartBPSetupClicked();
	FReply OnInspectVariablesClicked();

	// UI Field References
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ResourceTypeComboBox;
	TArray<TSharedPtr<FString>> ResourceTypeOptions;

	// Data Flow Helpers
	FKaliBlueprintSetupData GatherCurrentUIData() const;
	void SendDataToModulePlugin(const FKaliBlueprintSetupData& Data);
};