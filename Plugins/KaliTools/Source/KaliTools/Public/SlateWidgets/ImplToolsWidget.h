#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
class SImplToolsTab : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SImplToolsTab) {}

		SLATE_ARGUMENT(TArray <TSharedPtr <FAssetData>>, AssetsDataToStore)

	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	TArray<TSharedPtr<FAssetData>> StoredAssetData;

	TSharedRef< SListView<TSharedPtr<FAssetData>>> ConstructAssetListView();
	TSharedPtr<SListView<TSharedPtr <FAssetData>>> ConstructedAssetListView; 
	void RefreshAssetListView();

	TSharedRef<ITableRow> OnGeneratedRowForList(TSharedPtr<FAssetData> AssetDataToDisplay,
		const TSharedRef<STableViewBase>& OwnerTable);

	TSharedRef<SCheckBox> ConstructCheckBox(const TSharedPtr<FAssetData>& AssetDataToDisplay);
	void OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData);

	TSharedRef<STextBlock> ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontToUse);

	TSharedRef<SButton> ConstructButtonRowWidget(const TSharedPtr<FAssetData>& AssetDataToDisplay);

	FReply OnDeleteButtonClicked(TSharedPtr<FAssetData> ClickedAssetData);

	FSlateFontInfo GetEmboseedTextFont() const { return FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));}

	// Holds the active sub-tab view index
	TSharedPtr<SWidgetSwitcher> TabWidgetSwitcher;

	/** Active sub-tab index tracker */
	int32 ActiveTabIndex = 0;


	FReply OnSubTabClicked(int32 TabIndex);

	/** Helper to build tab navigation buttons */
	TSharedRef<SWidget> BuildTabButton(const FString& TabName, int32 TabIndex);


};