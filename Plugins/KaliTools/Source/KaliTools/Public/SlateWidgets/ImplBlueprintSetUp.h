#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"

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

	/** ListView Row Generator Callback */
	TSharedRef<ITableRow> OnGenerateAssetRow(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable);

	/** Refresh the UI list view */
	void RefreshListView();
};